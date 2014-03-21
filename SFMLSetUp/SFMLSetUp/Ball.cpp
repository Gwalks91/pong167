
#include "Ball.h"

Ball::Ball(float speed, sf::Texture t)
{
	ballSpeed = speed;
	velocity = sf::Vector2f(ballSpeed, ballSpeed);
	position = sf::Vector2f(SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
	pTexture = t;
	pSprite.setTexture(pTexture);
	pSprite.setPosition(position);

	mult = 1.0f;

	destination = position;
	nextDestination = sf::Vector2f(9999.0f,9999.0f);

	newPosition = sf::Vector2f(position.x, position.y);
	goingRight = true;
	reachLastServerPosition = true;
}

Ball::~Ball()
{
}

void Ball::ChangeBallDirection()
{
	if(!NETWORKED)
	{
		velocity.x *= (-1.0f);
	}
	else
	{
		velocity = sf::Vector2f(0.0f, 0.0f);
	}
}

sf::FloatRect Ball::GetSpriteBoundingBox()
{
	return pSprite.getGlobalBounds();
}

bool Ball::GetDirection()
{
	return goingRight;
}

void Ball::ResetBall()
{
	position = sf::Vector2f(SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
	pSprite.setPosition(position.x, position.y);
}

void Ball::Update(float elapsedTime)
{
	if(DistanceBetweenVectors(destination, position) != 0 && reachLastServerPosition)
	{
		sf::Vector2f difference = destination - position;
		NormalizeVector(difference);
		position = destination - (elapsedTime * difference);
	}

	CheckBounds();
		
	if(velocity.x >= 0)
		goingRight = true;
	else
		goingRight = false;

	pSprite.setPosition(position.x, position.y);
}

void Ball::Draw(sf::RenderWindow* w)
{
	w->draw(pSprite);
}

void Ball::CheckBounds()
{
	if((velocity.y > 0 && position.y > SCREEN_HEIGHT - pSprite.getGlobalBounds().height) 
		|| (velocity.y < 0 && position.y < 0))
	{
		if(!NETWORKED)
		{
			velocity.y *= -1.0f;
		}
		else
		{
			velocity = sf::Vector2f(0.0f, 0.0f);
		}
		
	}
}

void Ball::ballDeadReck(sf::Vector2f deadReckVelocity, sf::Vector2f old_Position, double latency)
{
	destination = old_Position;
	velocity = deadReckVelocity;
	reachLastServerPosition = true;
}

sf::Vector2f Ball::lerp(sf::Vector2f start, sf::Vector2f end, float percent)
{
	return (start + percent*(end - start));
}