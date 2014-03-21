#include <SFML/System.hpp> //For Mutexs

#include "Server.h"

#include <sstream>

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>

//Testing
#include <iostream>
#include <string>

//The local copy of the game state.

static const int SCREEN_WIDTH = 1000;
static const int SCREEN_HEIGHT = 550;

static const int BALL_WIDTH = 20;
static const int BALL_HEIGHT = 20;
sf::Vector2f ballVelocity(3.0f, 3.0f);

static const int PADDLE_WIDTH = 23;
static const int PADDLE_HEIGHT = 66;
sf::Vector2f paddle1Velocity(0.0f, 0.0f);
sf::Vector2f paddle2Velocity(0.0f, 0.0f);

static const int LEFT_WALL = 0;
static const int RIGHT_WALL = 1000;

static const int MAX_POINTS = 10;
int player1Points = 0;
int player2Points = 0;


//These are the paddles and balls placed initiated in their starting location.
sf::FloatRect ballRect = sf::FloatRect(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, BALL_WIDTH, BALL_HEIGHT);
sf::FloatRect onePaddleRect = sf::FloatRect(10.0f, 0.0f, PADDLE_WIDTH, PADDLE_HEIGHT);
sf::FloatRect twoPaddleRect = sf::FloatRect(970.0f, 0.0f, PADDLE_WIDTH, PADDLE_HEIGHT);

//Need to use a external main Mutex to lock thread unsafe objects.
//Similar to lock(not the same from what I understand) but will 
//stop other threads from accessing locked pieces of code at the same time.
sf::Mutex* serverMutex = new sf::Mutex;

//The amount of time it takes to send a message in one direction.
int playerOneLag = 0;
int playerTwoLag = 0;

Server server;

//The clock to calculate the time.
sf::Clock deltaClock;

sf::Clock ballClock;

sf::Thread* stateThread;

double timePassed = 0;

bool ballCollisionDetected = false;

float ballSendTimer = 0;

void checkBounds()
{
	if((ballVelocity.y > 0 && ballRect.top > SCREEN_HEIGHT - ballRect.height) || (ballVelocity.y < 0 && ballRect.top < 0))
	{
		ballCollisionDetected = true;
		ballVelocity.y *= -1.0f;
	}
	if(ballRect.top < 100 || ballRect.top + ballRect.height > SCREEN_HEIGHT - 100 || ballRect.left < 100 || ballRect.left + ballRect.width > SCREEN_WIDTH - 100)
	{
		ballCollisionDetected = true;
	}
}

bool checkBoundsPosition(sf::Vector2f pos)
{
	if((pos.y <= 0) || (pos.y >= SCREEN_HEIGHT - PADDLE_HEIGHT))
	{
		return false;
	}
	return true;
}

void stateUpdate()
{
	while(server.hasGameStarted())
	{
		float elapsedTime = deltaClock.getElapsedTime().asMilliseconds();
		float ballElapsedTime = ballClock.getElapsedTime().asMilliseconds();
		ballSendTimer += elapsedTime;
		ballClock.restart();
		if(elapsedTime >= 16.6)
		{
			deltaClock.restart();
			//Update the ball's position.
			ballRect = sf::FloatRect(ballRect.left + ballVelocity.x, ballRect.top + ballVelocity.y, ballRect.width, ballRect.height);
		}

		//Check for paddle collisions.
		if(ballRect.intersects(onePaddleRect) && ballVelocity.x < 0)
		{
			ballVelocity.x *= (-1.0f);
			std::cout << "Ball rebound off Paddle One." << std::endl;
			ballCollisionDetected = true;
		}
		else if(ballRect.intersects(twoPaddleRect) && ballVelocity.x > 0)
		{
			ballVelocity.x *= (-1.0f);
			std::cout << "Ball rebound off Paddle Two." << std::endl;
			ballCollisionDetected = true;
		}

		checkBounds();

		//Check and manage points for both players.
		if(ballRect.left <= LEFT_WALL + 3 && ballVelocity.x < 0)
		{
			server.sendAll("3 2");
			
			player2Points++;

			std::cout << "Player 2 Score update: " << player2Points << std::endl;

			//Send the end game message if player 2 wins.
			if(player2Points >= MAX_POINTS)
			{
				server.sendAll("4 2");
				
				server.stopGame();

				std::cout << "Player Two win with points." << std::endl;
			}

			ballRect = sf::FloatRect(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, BALL_WIDTH, BALL_HEIGHT);
			ballCollisionDetected = true;
		}
		if(ballRect.left + ballRect.width >= SCREEN_WIDTH - 3 && ballVelocity.x > 0)
		{
			server.sendAll("3 1");

			player1Points++;

			std::cout << "Player 1 Score update: " << player1Points << std::endl;

			//Send the end game message if player 1 wins.
			if(player1Points >= MAX_POINTS)
			{
				server.sendAll("4 1");

				server.stopGame();

				std::cout << "Player One win with points." << std::endl;
			}

			ballRect = sf::FloatRect(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, BALL_WIDTH, BALL_HEIGHT);
			ballCollisionDetected = true;
		}

		if(ballCollisionDetected || ballSendTimer >= (1/30))
		{
			ballSendTimer = 0;
			std::time_t rawtime;
			std::time(&rawtime);

			//Send the ball packet.
			std::stringstream ballPacketStringStream;
			
			ballPacketStringStream << "1 " << ballRect.left << " " << ballRect.top << " " << ballVelocity.x << " " << ballVelocity.y << " " << (rawtime - ((playerOneLag + playerTwoLag)/2));

			server.sendAll(ballPacketStringStream.str());

			ballCollisionDetected = false;
		}
	}
}

int main()
{
	//Create and connect server to port.

	server.connect(4455);
	//Set Mutex.
	server.setMutex(serverMutex);

	//Simulates the update loop of game.
	while(server.isConnected())
	{
		if(server.hasGameStarted())
		{
			while(!server.isPacketQueueEmpty())
			{
				sf::Packet packetToProcess = server.getTopPacket();

				std::string stringReceived;
				packetToProcess >> stringReceived;

				//Grab the number from the string and set up parsing helpers.
				int currentPos = 0;
				int posOfNextSpace = 0;
				std::string subString = "";

				posOfNextSpace = stringReceived.find(" ");
				//Place the number in the substring.
				subString = stringReceived.substr(currentPos, posOfNextSpace);

				//Do the if else statements to figure out what needs to be down here.
				if(atoi(subString.c_str()) == 0)
				{
					currentPos = posOfNextSpace;
					posOfNextSpace = stringReceived.find(" ", currentPos + 1);
					//Place the player number in the substring.
					subString = stringReceived.substr(currentPos, posOfNextSpace);
					int clientNumber = atoi(subString.c_str());

					currentPos = posOfNextSpace;
					posOfNextSpace = stringReceived.find(" ", currentPos + 1);
					//Find out which is a somputer and which is a client.
					subString = stringReceived.substr(currentPos, posOfNextSpace);
					std::istringstream stream(subString);
					time_t clientSentTime;
					stream >> clientSentTime;

					//Calculate lag.
					if(clientNumber == 1)
					{
						playerOneLag = clientSentTime - server.getFirstSentPacketTime();
						
						std::cout << "Got a start game packet.  Player 1 lag: " << playerOneLag << std::endl;
					}
					else
					{
						playerTwoLag = clientSentTime - server.getFirstSentPacketTime();
						
						std::cout << "Got a start game packet.  Player 2 lag: " << playerTwoLag << std::endl;
					}
				}

				//If it is a movement update then send it to the other client.
				if(atoi(subString.c_str()) == 2)
				{
					currentPos = posOfNextSpace;
					posOfNextSpace = stringReceived.find(" ", currentPos + 1);
					//Place the player number in the substring.
					subString = stringReceived.substr(currentPos, posOfNextSpace);
					int playerNumber = atoi(subString.c_str());

					//Get the position.
					currentPos = posOfNextSpace;
					posOfNextSpace = stringReceived.find(" ", currentPos + 1);
					subString = stringReceived.substr(currentPos, posOfNextSpace);
					float xPosition = std::stof(subString.c_str());
				
					currentPos = posOfNextSpace;
					posOfNextSpace = stringReceived.find(" ", currentPos + 1);
					subString = stringReceived.substr(currentPos, posOfNextSpace);
					float yPosition = std::stof(subString.c_str());

					//Get the velocity.
					currentPos = posOfNextSpace;
					posOfNextSpace = stringReceived.find(" ", currentPos + 1);
					subString = stringReceived.substr(currentPos, posOfNextSpace);
					float xVelocity = std::stof(subString.c_str());

					currentPos = posOfNextSpace;
					posOfNextSpace = stringReceived.find(" ", currentPos + 1);
					subString = stringReceived.substr(currentPos, posOfNextSpace);
					float yVelocity = std::stof(subString.c_str());

					std::time_t rawtime;
					std::time(&rawtime);

					//Pass on the position to the opposite player.
					//The left paddle.
					if(playerNumber == 1)
					{
						std::stringstream serverPaddleString;
						serverPaddleString << "2 1 " << xPosition << " " << yPosition << " " << xVelocity << " " << yVelocity << " " << (rawtime - playerOneLag); 
						server.send(serverPaddleString.str(), 2);

						onePaddleRect = sf::FloatRect(xPosition, yPosition, PADDLE_WIDTH, PADDLE_HEIGHT);
						paddle1Velocity = sf::Vector2f(xVelocity, yVelocity);
					}
					//The right paddle.
					else
					{
						std::stringstream serverPaddleString;
						serverPaddleString << "2 2 " << xPosition << " " << yPosition << " " << xVelocity << " " << yVelocity << " " << (rawtime - playerTwoLag); 
						server.send(serverPaddleString.str(), 1);

						twoPaddleRect = sf::FloatRect(xPosition, yPosition, PADDLE_WIDTH, PADDLE_HEIGHT);
						paddle2Velocity = sf::Vector2f(xVelocity, yVelocity);
					}
				}

			}//Mutex unlocks here.

			//If ball thread has not yet started the we start it.
			if(stateThread == nullptr)
			{
				std::cout << "Starting update thread." << std::endl;
				deltaClock.restart();
				ballClock.restart();
				stateThread = new sf::Thread(&stateUpdate);
				stateThread->launch();
			}
		}
	}

	//Server exits and dies.
	delete serverMutex;
	stateThread->terminate();
	delete stateThread;

	std::cout << "Exit." << std::endl;

    return 0;
}