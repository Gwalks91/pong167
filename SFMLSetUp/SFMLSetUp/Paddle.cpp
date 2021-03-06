#include "Paddle.h"

#include <sstream>

//Testing
#include <iostream>

Paddle::Paddle(bool server, sf::Vector2f v, float speed, sf::Texture t)
{
	velocity = sf::Vector2f(0.0f, 0.0f);
	paddleSpeed = speed;
	position = v;
	pTexture = t;
	pSprite.setTexture(pTexture);
	pSprite.setPosition(position);
	oldPosition = position;
	buttonHeld = false;
	serverControl = server;
	doneFollowingServer = true;
}

Paddle::~Paddle()
{
}

//Moce the paddle based off of the input enum
void Paddle::MovePaddle(Input i, float deltaTime)
{
	if(!serverControl)
	{
		if(i != NoInput)
		{
			buttonHeld = true;
		
			velocity = sf::Vector2f(0.0f, paddleSpeed*(float)i);
		}
		else
		{
			buttonHeld = false;
		}
	}
}

sf::FloatRect Paddle::GetSpriteBoundingBox()
{
	return pSprite.getGlobalBounds();
}

//Going to have to check the collider 
void Paddle::Update(float elapsedTime)
{
	if(!serverControl)
	{
		//Set the position.
		position += velocity; //original position
	
		if(CheckBoundsPosition(position)) 
		{
			//Set position if move was valid.
			pSprite.setPosition(position);
		}
		else
		{
			//Reset the position if the move was invalid.
			position -= velocity;
			pSprite.setPosition(position);
		}

		if(!buttonHeld)
		{
			velocity = sf::Vector2f(0.0f, 0.0f);
		}
	}
	else
	{
		if(!doneFollowingServer)
		{
			//Slow the paddle down as it gets to the destination.
			if(position.y > destination.y)
			{
				velocity = sf::Vector2f(0, -3.0f);
			}
			else
			{
				velocity = sf::Vector2f(0, 3.0f);
			}

			float mult = 1.0f;

			if(DistanceBetweenVectors(position, destination)/distanceOfDead > .5)
			{
				mult = 2.0f;
			}

			//Move towards the new position set by the server.
			position += velocity * mult;
			if(DistanceBetweenVectors(position, destination) <= 3 || !CheckBoundsPosition(position))
			{
				position = destination;
				doneFollowingServer = true;
			}
		}
	}
	pSprite.setPosition(position);
}

void Paddle::Draw(sf::RenderWindow* w)
{
	w->draw(pSprite);
}

bool Paddle::CheckBounds(Input i)
{
	if((position.y <= 0 && i == -1) || (position.y >= SCREEN_HEIGHT - pSprite.getGlobalBounds().height && i == 1))
	{
		return false;
	}
	return true;
}


void Paddle::paddleDeadReck(sf::Vector2f deadReckVelocity, sf::Vector2f old_Position, double latency)
{
	//Set new position as a unit vector that will allow us to path towards it in the
	//update loop.
	destination = old_Position;
	
	distanceOfDead = DistanceBetweenVectors(position, destination);

	doneFollowingServer = false;
}

bool Paddle::CheckBoundsPosition(sf::Vector2f pos)
{
	if((pos.y <= 0) || (pos.y >= SCREEN_HEIGHT - pSprite.getGlobalBounds().height))
	{
		return false;
	}
	return true;
}

std::string Paddle::getPositionAndVelocityString()
{
	//If there is a significant change in position then we send an update to the server.
	if(abs(position.y - oldPosition.y) >= 3)
	{
		std::stringstream positionAndVelocity;
		positionAndVelocity << position.x << " " << position.y << " " << velocity.x << " " << velocity.y;
		oldPosition = position;
		return positionAndVelocity.str();
	}
	else
	{
		return "";
	}
}