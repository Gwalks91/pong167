#include "Paddle.h"

const float Paddle::accel = .01;

Paddle::Paddle(sf::Vector2f v, float speed, sf::Texture t)
{
	velocity = sf::Vector2f(0.0f, 0.0f);
	paddleSpeed = speed;
	currentSpeed = 0;
	position = v;
	pTexture = t;
	pSprite.setTexture(pTexture);
	pSprite.setPosition(position);
	buttonHeld = false;
}

//Moce the paddle based off of the input enum
void Paddle::MovePaddle(Input i)
{
	if(i != NoInput)
	{
		buttonHeld = true;
		
		//Inc. the speed by the acceleration rate.
		if(abs(currentSpeed) < .5f)
		{
			currentSpeed += accel * (float)i;
			//Set the velocity.
			velocity = sf::Vector2f(0.0f, currentSpeed);
		}
	}
	else
	{
		buttonHeld = false;
	}
}

sf::FloatRect Paddle::GetSpriteBoundingBox()
{
	return pSprite.getGlobalBounds();
}

//Going to have to check the collider 
void Paddle::Update(sf::Time deltaTime)
{
	//Set the position.
	position += velocity;
	
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
		//Slow the paddle down.		
		if(std::abs(currentSpeed) > 0.0001)
		{
			if(currentSpeed > 0)
			{
				currentSpeed -= accel;
			}
			else
			{
				currentSpeed += accel;
			}
			velocity = sf::Vector2f(0.0f, currentSpeed);
		}
		else
		{
			velocity = sf::Vector2f(0.0f, 0.0f);
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

bool Paddle::CheckBoundsPosition(sf::Vector2f pos)
{
	if((pos.y <= 0) || (pos.y >= SCREEN_HEIGHT - pSprite.getGlobalBounds().height))
	{
		return false;
	}
	return true;
}
