
#include "Engine.h"


Engine::Engine()
	:s(),
	wasHit(false),
	startGame(true)
{
	window = new sf::RenderWindow(sf::VideoMode(1000, 550), "SFML works!");

	player1 = new Player(1, sf::Vector2f(10.0f, 0.0f), sf::Keyboard::W, sf::Keyboard::S);
	player2 = new Player(2, sf::Vector2f(970.0f, 0.0f), sf::Keyboard::Up, sf::Keyboard::Down);

	ball = new Ball(300.0f, LoadTexture("PongBall.png"));

	backGround = LoadTexture("PokemonStadium.png");
	backgroundSprite.setTexture(backGround);
	backgroundSprite.setScale(SCREEN_WIDTH/backgroundSprite.getLocalBounds().width , SCREEN_HEIGHT/backgroundSprite.getLocalBounds().height);

	deltaClock = sf::Clock();
}

Engine::~Engine()
{
	delete player1;
	delete player2;
	delete ball;
	delete window;
}

void Engine::StartGame()
{
	startGame = true;
}

void Engine::EndGame(int playerID)
{
	startGame = false;
	switch (playerID)
	{
	case 1:
		//Show a texture for the player 1 win
		break;
	case 2:
		//Show a texture for the player 2 win
		break;

	}
}

void Engine::Update()
{	
	float elapsedTime = deltaClock.getElapsedTime().asSeconds();
	deltaClock.restart();
	
	if(startGame)
	{
		HandleInput();

		CheckCollision();

		player1->Update(elapsedTime);
		player2->Update(elapsedTime);
		ball->Update(elapsedTime);

		s.Update(elapsedTime);
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
		s.ChangeScore(2);
		ball->ResetBall();
	}
	if(ball->GetSpriteBoundingBox().left + ball->GetSpriteBoundingBox().width >= SCREEN_WIDTH)
	{
		s.ChangeScore(1);
		ball->ResetBall();
	}

}
