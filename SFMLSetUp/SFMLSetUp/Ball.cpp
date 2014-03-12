
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

	//latency to test deadreck
	latency = 200.95; //simulating latency
	newPosition = sf::Vector2f(0.0f,0.0f);

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
	pSprite.setPosition(position.x, position.y);
}

void Ball::Update(float elapsedTime)
{
	CheckBounds();
	if(velocity.x >= 0)
		goingRight = true;
	else
		goingRight = false;
	
	//old Position
	position += velocity * elapsedTime;
	//pSprite.setPosition(position.x, position.y);
	//deadreck takes in elapsed velocity with latency and uses it on  old_Position
	ballDeadReck(velocity*elapsedTime, position, latency);
	pSprite.setPosition(newPosition.x, newPosition.y);



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
	//takes in the old position and merges it with the velocity and latency
	newPosition = old_Position + deadReckVelocity*float(latency);

	//newDirection = 

	/*//Dead reck code

	//Time based on system
	time_t now = time(0);
	struct tm * ptm;

	//convert to utc
	time(&now);
	ptm = gmtime(&now);

	//utc hour and minutes
	int t1Hour = (ptm->tm_hour+UTC)%24;
	int t1Minutes = (ptm->tm_min);
	//?? need different time for this part
	int t4Hour = (ptm->tm_hour+UTC)%24;
	int t4Minutes = (ptm->tm_min);
	
	
	//does latency
	int latency = ((t4Hour - t1Hour) + (t4Minutes - t1Minutes))/2;
	newPosition = position;// + //(deadReckVelocity * (latency));*/
}

//Function that makes a new Vector2f in a random direction that is normalized
sf::Vector2f Ball::MakeRandomVector()
{
	sf::Vector2f newDirection(std::rand(), std::rand());
	NormalizeVector(newDirection);
	return newDirection;
}