
#ifndef PADDLE_H
#define PADDLE_H

#include <SFML/Graphics.hpp>
#include "Globals.h"
#include <iostream>

class Paddle
{
public :
	//Paddle();
	Paddle(bool server, sf::Vector2f v, float speed, sf::Texture t);
	~Paddle();
	void MovePaddle(Input i, float deltaTime);
	sf::FloatRect GetSpriteBoundingBox();
	void Update(float elapsedTime);
	void Draw(sf::RenderWindow* w);
	void paddleDeadReck(sf::Vector2f velocity, sf::Vector2f position, double latency);
	inline void setAsServer()
	{ serverControl = true; }
private:
	bool CheckBounds(Input i);
	bool CheckBoundsPosition(sf::Vector2f pos);
	bool buttonHeld;
	bool serverControl;
	bool doneFollowingServer;
	sf::Texture pTexture;
	sf::Sprite pSprite;
	sf::Vector2f position;
	sf::Vector2f velocity;
	float paddleSpeed;
	float currentSpeed;

	sf::Vector2f newPosition;

	static const float max;
};

#endif