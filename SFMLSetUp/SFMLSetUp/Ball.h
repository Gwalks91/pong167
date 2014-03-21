
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
	sf::Vector2f lerp(sf::Vector2f start, sf::Vector2f end, float percent);
private:
	void CheckBounds();
	float ballSpeed;
	bool goingRight;
	bool reachLastServerPosition;
	sf::Texture pTexture;
	sf::Sprite pSprite;
	sf::Vector2f position;
	sf::Vector2f velocity;
	sf::Vector2f nextVelocity;
	sf::Vector2f deadReckVel;
	sf::Vector2f newPosition;
	sf::Vector2f destination;
	sf::Vector2f nextDestination;
	float deadDistance;
	float mult;
	float currentLatency;
};

#endif