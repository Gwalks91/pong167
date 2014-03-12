#include "Paddle.h"

const float Paddle::max = .3f;

Paddle::Paddle(bool server, sf::Vector2f v, float speed, sf::Texture t)
{
	velocity = sf::Vector2f(0.0f, 0.0f);
	paddleSpeed = speed;
	currentSpeed = 0;
	position = v;
	pTexture = t;
	pSprite.setTexture(pTexture);
	pSprite.setPosition(position);
	buttonHeld = false;
	serverControl = server;
	doneFollowingServer = true;

	//Deadreck
	newPosition = sf::Vector2f (0.0f,0.0f);
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
	if(!NETWORKED)
	{
		//Set the position.
		position += velocity * elapsedTime; //original position
	
		if(CheckBoundsPosition(position)) 
		{
			//Set position if move was valid.
			pSprite.setPosition(position);
		}
		else
		{
			//Reset the position if the move was invalid.
			position -= velocity * elapsedTime;
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
			//Move towards the new position set by the server.
			position += position * (newPosition.x * velocity.y - newPosition.y * velocity.x) * elapsedTime;
			//Stop moving when we are close enough to the server.
			if(DistanceBetweenVectors(position, newPosition) == .0001)
			{
				position = newPosition;
				velocity = sf::Vector2f(0.0f, 0.0f);
				doneFollowingServer = true;
			}

			pSprite.setPosition(position.x, position.y);
		}
		else
		{
			position = position;
		}
	}
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
	newPosition = sf::Vector2f(old_Position.x - position.x, old_Position.y - position.y);
	NormalizeVector(newPosition);

	//Set the velocity to catch up with the lag. TO-DO
	velocity = sf::Vector2f(0, deadReckVelocity.y);

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
