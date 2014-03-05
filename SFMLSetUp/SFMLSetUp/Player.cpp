
#include "Player.h"
#include <string>

Player::Player(int pID, sf::Vector2f paddlePos, sf::Keyboard::Key up, sf::Keyboard::Key down)
	:playerID(pID),
	moveUp(up),
	moveDown(down),
	score(0)
{
	p = new Paddle(paddlePos, 600.0f, LoadTexture("paddle.png"));
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
	//text.setString("Score: " + GetScore());
	handleInput(elapsedTime);
	//Replace this dummy time with the actual one.
	p->Update(elapsedTime);
}

void Player::Draw(sf::RenderWindow* w)
{
	w->draw(text);
	p->Draw(w);
}
