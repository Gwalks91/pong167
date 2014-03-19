
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
	~Ball();
	sf::FloatRect GetSpriteBoundingBox(); 
	void ChangeBallDirection();
	bool GetDirection();
	void ResetBall();
	void Update(float elapsedTime);
	void Draw(sf::RenderWindow* w);
	void ballDeadReck(sf::Vector2f position, sf::Vector2f velocity, double latency);
	float getBallVelocity()
	{ return velocity.y; }
private:
	void CheckBounds();
	bool CheckCollision();
	float ballSpeed;
	bool goingRight;
	sf::Texture pTexture;
	sf::Sprite pSprite;
	sf::Vector2f position;
	sf::Vector2f velocity;
	float deadVelocity;
	sf::Vector2f newPosition;
	sf::Vector2f destination;
	int mult;
};

#endif