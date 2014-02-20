
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
	void MovePaddle(Input i);
	void Update();
	void Draw(sf::RenderWindow* w);
private:
	bool CheckBounds(Input i);
	sf::Texture pTexture;
	sf::Sprite pSprite;
	sf::Vector2f position;
	sf::Vector2f velocity;
	
};

#endif