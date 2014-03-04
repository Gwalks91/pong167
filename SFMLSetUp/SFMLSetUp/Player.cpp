
#include "Player.h"
#include <string>

Player::Player(int pID, sf::Vector2f paddlePos, sf::Keyboard::Key up, sf::Keyboard::Key down)
{
	playerID = pID;

	p = new Paddle(paddlePos, .5f, LoadTexture("paddle.png"));
	moveUp = up;
	moveDown = down;

	score = 0;

	font = GetFont("ALGER.ttf");
	text.setFont(font);
	text.setPosition(200,0);
	text.setCharacterSize(12);
}

int Player::GetID()
{
	return playerID;
}

Paddle* Player::GetPaddle()
{
	return p;
}

void Player::handleInput()
{
	bool up = false;
	bool down = false;
	
	//Add dead Reckoning
	//Send a message to the server to update the main game 
	if (sf::Keyboard::isKeyPressed(moveUp))
	{
		p->MovePaddle(Input::MoveUp);
		up = true;
	}
	if(sf::Keyboard::isKeyPressed(moveDown))
	{
		p->MovePaddle(Input::MoveDown);
		down = true;
	}

	if(!up && !down)
	{
		p->MovePaddle(Input::NoInput);
	}
}

int Player::GetScore()
{
	return score;
}

void Player::Update()
{
	//text.setString("Score: " + GetScore());
	handleInput();
	//Replace this dummy time with the actual one.
	sf::Time time;
	p->Update(time);
}

void Player::Draw(sf::RenderWindow* w)
{
	w->draw(text);
	p->Draw(w);
}

sf::Font Player::GetFont(std::string fontFile)
{
	sf::Font f;
	if(!f.loadFromFile(fontFile))
	{
		std::cout << "Font, " << fontFile << " was not loaded." << std::endl;
	}
	return f;
}

sf::Texture Player::LoadTexture(std::string s)
{
	sf::Texture t;
	if(!t.loadFromFile(s))
	{
		std::cout << "An Error has occured!" << std::endl;
	}
	return t;
}