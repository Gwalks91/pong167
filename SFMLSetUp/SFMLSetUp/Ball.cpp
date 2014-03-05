
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

	goingRight = true;
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
}

void Ball::Update(float elapsedTime)
{
	CheckBounds();
	if(velocity.x >= 0)
		goingRight = true;
	else
		goingRight = false;
	position += velocity * elapsedTime;
	pSprite.setPosition(position.x, position.y);
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