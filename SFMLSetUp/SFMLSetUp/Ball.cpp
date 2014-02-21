
#include "Ball.h"


Ball::Ball(float speed, sf::Texture t)
{
	std::srand(std::time(NULL));

	ballSpeed = speed;
	velocity = MakeRandomVector() * speed;
	position = sf::Vector2f(SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
	pTexture = t;
	pSprite.setTexture(pTexture);
	pSprite.setPosition(position);
}

void Ball::ChangeBallDirection()
{
	bool goingRight = false;
	if(velocity.x > 0)
	{
		goingRight = true;
	}
	velocity = MakeRandomVector() * ballSpeed;
	if(goingRight)
		velocity *= (-1.0f);
}

sf::FloatRect Ball::GetSpriteBoundingBox()
{
	return pSprite.getGlobalBounds();
}

void Ball::Update()
{
	CheckBounds();
	position += velocity;
	pSprite.setPosition(position);
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

//Function that makes a new Vector2f in a random direction that is normalized
sf::Vector2f Ball::MakeRandomVector()
{
	sf::Vector2f newDirection(std::rand(), std::rand());
	NormalizeVector(newDirection);
	return newDirection;
}