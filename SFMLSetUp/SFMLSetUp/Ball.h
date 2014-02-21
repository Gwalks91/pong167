
#ifndef BALL_H
#define BALL_H

#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Globals.h"
#include <time.h>

class Ball
{
public:
	Ball(float speed, sf::Texture t);
	sf::FloatRect GetSpriteBoundingBox(); 
	void ChangeBallDirection();
	void Update();
	void Draw(sf::RenderWindow* w);
private:
	sf::Vector2f MakeRandomVector();
	void CheckBounds();
	bool CheckCollision();
	float ballSpeed;
	sf::Texture pTexture;
	sf::Sprite pSprite;
	sf::Vector2f position;
	sf::Vector2f velocity;
};

#endif