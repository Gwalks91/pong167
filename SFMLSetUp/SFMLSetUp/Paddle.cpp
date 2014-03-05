#include "Paddle.h"

const float Paddle::max = .3f;

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

Paddle::~Paddle()
{

}

//Moce the paddle based off of the input enum
void Paddle::MovePaddle(Input i, float deltaTime)
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

sf::FloatRect Paddle::GetSpriteBoundingBox()
{
	return pSprite.getGlobalBounds();
}

//Going to have to check the collider 
void Paddle::Update(float elapsedTime)
{
	//Set the position.
	position += velocity * elapsedTime;
	
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
