
#include "Engine.h"


Engine::Engine()
{
	window = new sf::RenderWindow(sf::VideoMode(1000, 550), "SFML works!");

	player1 = new Player(1, sf::Vector2f(10.0f, 0.0f), sf::Keyboard::W, sf::Keyboard::S);
	player2 = new Player(2, sf::Vector2f(970.0f, 0.0f), sf::Keyboard::Up, sf::Keyboard::Down);

	ball = new Ball(0.1f, LoadTexture("PongBall.png"));

	wasHit = false;
}

void Engine::Update()
{	
	sf::Time elapsedTime = clock.getElapsedTime();

	HandleInput();

	CheckCollision();

	player1->Update();
	player2->Update();
	ball->Update();

}
	
void Engine::Draw()
{
	window->clear();
	player1->Draw(window);
	player2->Draw(window);
	ball->Draw(window);
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
					std::cout << "the escape key was pressed" << std::endl;
				}
				player1->HandleInput(event);
				player2->HandleInput(event);
				break;
		}
	}
}

void Engine::CheckCollision()
{
	if(ball->GetSpriteBoundingBox().intersects(player1->GetPaddle()->GetSpriteBoundingBox()) && !ball->GetDirection())
	{
		ball->ChangeBallDirection();
	}
		
	if(ball->GetSpriteBoundingBox().intersects(player2->GetPaddle()->GetSpriteBoundingBox()) && ball->GetDirection())
	{
		ball->ChangeBallDirection();
	}
}
