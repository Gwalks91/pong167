#ifndef PLAYER_H
#define PLAYER_H


#include "Paddle.h"
#include "Globals.h"
#include <SFML/Graphics.hpp>

class Player
{
public:
	sf::Keyboard::Key moveUp;
	sf::Keyboard::Key moveDown;
	Player(int pID, sf::Vector2f paddlePos, sf::Keyboard::Key up, sf::Keyboard::Key down);
	~Player();
	int GetID();
	Paddle* GetPaddle();
	void Update(float elapsedTime);
	void Draw(sf::RenderWindow* w);
	void setAsServer()
	{ p->setAsServer(); }
	void deadReck(sf::Vector2f velocity, sf::Vector2f position, double latency)
	{ p->paddleDeadReck(velocity, position, latency); }
private:
	int playerID;
	int score;
	Paddle* p;
	int GetScore();
	void handleInput(float deltaTime);
};

#endif