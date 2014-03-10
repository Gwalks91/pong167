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

	//deadreck
	latency = 400.0; //simulating latency
	newPosition = sf::Vector2f (0.0f,0.0f);
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
	position += velocity * elapsedTime; //original position
	
	
	if(CheckBoundsPosition(newPosition)) 
	{
		//Set position if move was valid.
		pSprite.setPosition(position);
		paddleDeadReck(velocity*elapsedTime, position, latency); //deadreck position
		pSprite.setPosition(newPosition);

	}
	else
	{
		//Reset the position if the move was invalid.
		position -= velocity * elapsedTime;
		pSprite.setPosition(position);
		paddleDeadReck(velocity*elapsedTime, position, latency);
		pSprite.setPosition(newPosition/*position*/);
		
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


void Paddle::paddleDeadReck(sf::Vector2f deadReckVelocity, sf::Vector2f old_Position, double latency)
{
	//paddle deadreck
	newPosition = old_Position + deadReckVelocity*float(latency);

}
bool Paddle::CheckBoundsPosition(sf::Vector2f pos)
{
	if((pos.y <= 0) || (pos.y >= SCREEN_HEIGHT - pSprite.getGlobalBounds().height))
	{
		return false;
	}
	return true;
}
