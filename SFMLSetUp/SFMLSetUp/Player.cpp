
#include "Player.h"
#include <string>

Player::Player(int pID, sf::Vector2f paddlePos, sf::Keyboard::Key up, sf::Keyboard::Key down)
	:playerID(pID),
	moveUp(up),
	moveDown(down),
	score(0)
{
	//Set the paddles to be the right facing one.
	if(playerID == 1)
	{
		p = new Paddle(false, paddlePos, 600.0f, LoadTexture("paddle1.png"));
	}
	else
	{
		p = new Paddle(false, paddlePos, 600.0f, LoadTexture("paddle2.png"));
	}

}

Player::~Player()
{
	delete p;
}

int Player::GetID()
{
	return playerID;
}

Paddle* Player::GetPaddle()
{
	return p;
}

void Player::handleInput(float deltaTime)
{
	bool up = false;
	bool down = false;
	
	//Add dead Reckoning
	//Send a message to the server to update the main game 
	if (sf::Keyboard::isKeyPressed(moveUp))
	{
		up = true;
	}
	if(sf::Keyboard::isKeyPressed(moveDown))
	{
		down = true;
	}

	if(!up && !down)
	{
		p->MovePaddle(Input::NoInput, deltaTime);
	}
	else if(up && down)
	{
		p->MovePaddle(Input::NoInput, deltaTime);
	}
	else if(down)
	{
		p->MovePaddle(Input::MoveDown, deltaTime);
	}
	else if(up)
	{
		p->MovePaddle(Input::MoveUp, deltaTime);
	}
}

int Player::GetScore()
{
	return score;
}

void Player::Update(float elapsedTime)
{
	handleInput(elapsedTime);
	p->Update(elapsedTime);
}

void Player::Draw(sf::RenderWindow* w)
{
	p->Draw(w);
}
