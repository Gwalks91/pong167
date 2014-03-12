
#include "Engine.h"

#include <time.h>
#include <sstream>

Engine::Engine()
	:s(),
	wasHit(false)
{
	window = new sf::RenderWindow(sf::VideoMode(1000, 550), "PokePong!");

	player1 = new Player(1, sf::Vector2f(10.0f, 0.0f), sf::Keyboard::W, sf::Keyboard::S);
	player2 = new Player(2, sf::Vector2f(970.0f, 0.0f), sf::Keyboard::Up, sf::Keyboard::Down);

	ball = new Ball(300.0f, LoadTexture("PongBall.png"));

	backGround = LoadTexture("PokemonStadium.png");
	backgroundSprite.setTexture(backGround);
	backgroundSprite.setScale(SCREEN_WIDTH/backgroundSprite.getLocalBounds().width , SCREEN_HEIGHT/backgroundSprite.getLocalBounds().height);
	
	clientThread = nullptr;

	if(!NETWORKED)
	{
		startGame = true;
	}
	else
	{
		//Try to connect to the server.
		clientMutex = new sf::Mutex;
		client = new Client("128.195.51.251", 4455);
		client->setMutex(clientMutex);
		startGame = false;

		clientThread = new sf::Thread(&clientUpdateThread);
		clientThread->launch();
	}
}

Engine::~Engine()
{
	delete player1;
	delete player2;
	delete ball;
	delete window;

	if(clientThread != nullptr)
	{
		clientThread->terminate();
		delete clientThread;
	}
}

void Engine::Update()
{	
	if(startGame)
	{	
		float elapsedTime = deltaClock.getElapsedTime().asSeconds();
		deltaClock.restart();

		CheckCollision();

		player1->Update(elapsedTime);
		player2->Update(elapsedTime);
		ball->Update(elapsedTime);

		s.Update(elapsedTime);

		//If we are online we want to send our information to the Server.
		if(NETWORKED)
		{
			std::stringstream paddleString;
			if(clientNumber == 1)
			{
				std::time_t rawtime;
				std::time(&rawtime);

				paddleString << "2 " << clientNumber << " " << player1->GetPaddle()->getPositionAndVelocityString() << " " << rawtime;
			}
			else
			{
				std::time_t rawtime;
				std::time(&rawtime);

				paddleString << "2 " << clientNumber << " " << player2->GetPaddle()->getPositionAndVelocityString() << " " << rawtime;
			}
		}

		if(s.winnerIs() != 0)
		{
			startGame = false;
		}
		HandleInput();
	}
}
	
void Engine::Draw()
{
	window->clear();

	window->draw(backgroundSprite);
	player1->Draw(window);
	player2->Draw(window);
	ball->Draw(window);
	s.Draw(window);
	if(s.winnerIs() == 1)
	{
		winScreen = LoadTexture("PlayerOneWin.png");
		winScreenSprite.setTexture(winScreen);
		winScreenSprite.setScale(SCREEN_WIDTH/backgroundSprite.getLocalBounds().width , SCREEN_HEIGHT/backgroundSprite.getLocalBounds().height);
		window->draw(winScreenSprite);
	}
	else if(s.winnerIs() == 2)
	{
		winScreen = LoadTexture("PlayerTwoWin.png");
		winScreenSprite.setTexture(winScreen);
		winScreenSprite.setScale(SCREEN_WIDTH/backgroundSprite.getLocalBounds().width , SCREEN_HEIGHT/backgroundSprite.getLocalBounds().height);
		window->draw(winScreenSprite);
	}
    window->display();
}


void Engine::HandleInput()
{
	sf::Event event;
    while (window->pollEvent(event))
	{
		// check the type of the event...
		switch (event.type)
		{
			// window closed
			case sf::Event::Closed:
				window->close();
				break;

			// key pressed
			case sf::Event::KeyPressed:
				if (event.key.code == sf::Keyboard::Escape)
				{
					window->close();
				}
				break;
		}
	}
}

void Engine::CheckCollision()
{
	if(ball->GetSpriteBoundingBox().intersects(player1->GetPaddle()->GetSpriteBoundingBox()) && ball->GetSpriteBoundingBox().left > player1->GetPaddle()->GetSpriteBoundingBox().left + player1->GetPaddle()->GetSpriteBoundingBox().width - 3 && !ball->GetDirection())
	{
		ball->ChangeBallDirection();
	}
		
	if(ball->GetSpriteBoundingBox().intersects(player2->GetPaddle()->GetSpriteBoundingBox()) && ball->GetSpriteBoundingBox().left + ball->GetSpriteBoundingBox().width - 3 < player2->GetPaddle()->GetSpriteBoundingBox().left && ball->GetDirection())
	{
		ball->ChangeBallDirection();
	}

	if(ball->GetSpriteBoundingBox().left <= 0)
	{
		if(!NETWORKED)
		{
			s.ChangeScore(2);
			ball->ResetBall();
		}
	}
	if(ball->GetSpriteBoundingBox().left + ball->GetSpriteBoundingBox().width >= SCREEN_WIDTH && ball->GetSpriteBoundingBox().left + ball->GetSpriteBoundingBox().width < SCREEN_WIDTH + 10)
	{
		if(!NETWORKED)
		{
			s.ChangeScore(1);
			ball->ResetBall();
		}
	}

}

void Engine::clientUpdateThread()
{
	if(NETWORKED)
	{
		while(!client->isPacketQueueEmpty())
		{
			sf::Packet recievedPacket = client->getTopPacket();
			std::string stringReceived;
			recievedPacket >> stringReceived;

			//Grab the number from the string and set up parsing helpers.
			int currentPos = 0;
			int posOfNextSpace = 0;
			std::string subString = "";

			posOfNextSpace = stringReceived.find(" ");
			//Place the number in the substring.
			subString = stringReceived.substr(currentPos, posOfNextSpace);

			if(atoi(subString.c_str()) == 0)
			{
				currentPos = posOfNextSpace;
				posOfNextSpace = stringReceived.find(" ", currentPos + 1);
				//Find out which is a somputer and which is a client->
				subString = stringReceived.substr(currentPos, posOfNextSpace);
				clientNumber = atoi(subString.c_str());

				//Start the game.
				startGame = true;
			
				//Sending the client time.
				std::time_t rawtime;
				std::time(&rawtime);

				std::stringstream clientStartStream;
				clientStartStream << "0 " << clientNumber << " " << rawtime;
				client->send(clientStartStream.str());
					
				if(clientNumber == 1)
				{
					player2->setAsServer();
				}
				else
				{
					player1->setAsServer();
				}

				//Start the main update loop and start message sending and recieving.
			}
			else if(atoi(subString.c_str()) == 1 || atoi(subString.c_str()) == 2)
			{
				//Update the ball/paddle and set new velocity and accel. 
				//for dead reackoning.
				//Function to set balls server position and velocity and interpolate towards it.
				int clientToDeadReck = 0;
				if(atoi(subString.c_str()) == 2)
				{
					currentPos = posOfNextSpace;
					posOfNextSpace = stringReceived.find(" ", currentPos + 1);
					subString = stringReceived.substr(currentPos, posOfNextSpace);
					clientToDeadReck = atoi(subString.c_str());
				}

				currentPos = posOfNextSpace;
				posOfNextSpace = stringReceived.find(" ", currentPos + 1);
				subString = stringReceived.substr(currentPos, posOfNextSpace);
				float xPosition = std::stof(subString.c_str());
				
				currentPos = posOfNextSpace;
				posOfNextSpace = stringReceived.find(" ", currentPos + 1);
				subString = stringReceived.substr(currentPos, posOfNextSpace);
				float yPosition = std::stof(subString.c_str());

				currentPos = posOfNextSpace;
				posOfNextSpace = stringReceived.find(" ", currentPos + 1);
				subString = stringReceived.substr(currentPos, posOfNextSpace);
				float xVelocity = std::stof(subString.c_str());

				currentPos = posOfNextSpace;
				posOfNextSpace = stringReceived.find(" ", currentPos + 1);
				subString = stringReceived.substr(currentPos, posOfNextSpace);
				float yVelocity = std::stof(subString.c_str());

				if(atoi(subString.c_str()) == 1)
				{
					ball->ballDeadReck(sf::Vector2f(xPosition, yPosition), sf::Vector2f(xVelocity, yVelocity), 0);
				}
				else
				{
					if(clientNumber == 1)
					{
						player2->deadReck(sf::Vector2f(xPosition, yPosition), sf::Vector2f(xVelocity, yVelocity), 0);
					}
					else
					{
						player1->deadReck(sf::Vector2f(xPosition, yPosition), sf::Vector2f(xVelocity, yVelocity), 0);
					}
				}
			}
			else if(atoi(subString.c_str()) == 3)
			{
				currentPos = posOfNextSpace;
				posOfNextSpace = stringReceived.find(" ", currentPos + 1);
				//Find out who got the point.
				subString = stringReceived.substr(currentPos, posOfNextSpace);
				int pointEarner = atoi(subString.c_str());

				if(pointEarner == 1)
				{	
					s.ChangeScore(1);
				}
				else
				{
					s.ChangeScore(2);
				}
			}
		}
	}
}