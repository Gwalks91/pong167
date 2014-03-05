
#ifndef PADDLE_H
#define PADDLE_H

#include <SFML/Graphics.hpp>
#include "Globals.h"
#include <iostream>

class Paddle
{
public :
	//Paddle();
	Paddle(sf::Vector2f v, float speed, sf::Texture t);
	~Paddle();
	void MovePaddle(Input i, float deltaTime);
	sf::FloatRect GetSpriteBoundingBox();
	void Update(float elapsedTime);
	void Draw(sf::RenderWindow* w);
private:
	bool CheckBounds(Input i);
	bool CheckBoundsPosition(sf::Vector2f pos);
	bool buttonHeld;
	sf::Texture pTexture;
	sf::Sprite pSprite;
	sf::Vector2f position;
	sf::Vector2f velocity;
	float paddleSpeed;
	float currentSpeed;

	static const float max;
};

#endif