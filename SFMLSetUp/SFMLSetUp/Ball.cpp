
#include "Ball.h"

Ball::Ball(float speed, sf::Texture t)
{
	std::srand(std::time(NULL));
	ballSpeed = speed;
	velocity = sf::Vector2f(ballSpeed, ballSpeed);
	position = sf::Vector2f(SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
	pTexture = t;
	pSprite.setTexture(pTexture);
	pSprite.setPosition(position);

	//deadreck
	newPosition = sf::Vector2f(0.0f,0.0f);

	goingRight = true;

	if(!NETWORKED)
	{
		doneFollowingServer = true;
	}
}

Ball::~Ball()
{
}

void Ball::ChangeBallDirection()
{
	velocity.x *= (-1.0f);
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
	if(doneFollowingServer)
	{
		//old Position
		position += velocity * elapsedTime;
		pSprite.setPosition(position.x, position.y);
		
		CheckBounds();
		
		if(velocity.x >= 0)
			goingRight = true;
		else
			goingRight = false;
	
		if(position.x < 0 || position.x > SCREEN_WIDTH || position.y < -10 || position.y > SCREEN_HEIGHT + 10)
		{
			ResetBall();
		}
	}
	if(!doneFollowingServer)
	{
		position += newPosition * deadVelocity * elapsedTime;
		if(DistanceBetweenVectors(position, destination) <= 3)
		{
			position = destination;
			velocity = sf::Vector2f(ballSpeed, ballSpeed);
			doneFollowingServer = true;
		}
		pSprite.setPosition(position.x, position.y);
	}
}

void Ball::Draw(sf::RenderWindow* w)
{
	w->draw(pSprite);
}

//Checks to see if the ball collieded with a paddle
bool Ball::CheckCollision()
{
	//pSprite.getGlobalBounds().intersects(
	return false;
}

void Ball::CheckBounds()
{
	if((velocity.y > 0 && position.y > SCREEN_HEIGHT - pSprite.getGlobalBounds().height) 
		|| (velocity.y < 0 && position.y < 0))
	{
		velocity.y *= -1.0f;
	}
}

void Ball::ballDeadReck(sf::Vector2f deadReckVelocity, sf::Vector2f old_Position, double latency)
{
	////paddle deadreck
	////Set the location from the server to be the 
	destination = old_Position;
	newPosition = old_Position - position;
	newPosition = sf::Vector2f(newPosition.x/DistanceBetweenVectors(old_Position, position), newPosition.y/DistanceBetweenVectors(old_Position, position));

	//Set the velocity to catch up with the lag. TO-DO
	deadVelocity = DistanceBetweenVectors(old_Position, position)/(DistanceBetweenVectors(old_Position, position)/(ballSpeed + latency));
	doneFollowingServer = false;
}

//Function that makes a new Vector2f in a random direction that is normalized
sf::Vector2f Ball::MakeRandomVector()
{
	sf::Vector2f newDirection(std::rand(), std::rand());
	NormalizeVector(newDirection);
	return newDirection;
}