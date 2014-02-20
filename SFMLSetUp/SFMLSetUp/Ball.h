
#ifndef BALL_H
#define BALL_H

#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Globals.h"

class Ball
{
public:
	Ball(float speed, sf::Texture t);
	void ChangeBallDirection();
	void Update();
	void Draw(sf::RenderWindow* w);
private:
	sf::Vector2f MakeRandomVector();
	bool CheckCollision();
	float ballSpeed;
	sf::Texture pTexture;
	sf::Sprite pSprite;
	sf::Vector2f position;
	sf::Vector2f velocity;
};

#endif