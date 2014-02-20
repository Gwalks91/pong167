
#include "Ball.h"


Ball::Ball(float speed, sf::Texture t)
{
	ballSpeed = speed;
	velocity = MakeRandomVector() * speed;
	position = sf::Vector2f(SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
	pTexture = t;
	pSprite.setTexture(pTexture);
	pSprite.setPosition(position);
}

void Ball::ChangeBallDirection()
{
	velocity = MakeRandomVector() * ballSpeed;
}

void Ball::Update()
{
	if(CheckCollision())
	{
		ChangeBallDirection();
	}
	std::cout << velocity.x << " : " << velocity.y << std::endl;
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

//Function that makes a new Vector2f in a random direction
sf::Vector2f Ball::MakeRandomVector()
{
	sf::Vector2f newDirection(std::rand() % 5, std::rand() % 5);
	float length = std::sqrt(std::pow(newDirection.x, 2) * std::pow(newDirection.y, 2));
	newDirection.x = newDirection.x/length;
	newDirection.y = newDirection.y/length;
	return newDirection;
}