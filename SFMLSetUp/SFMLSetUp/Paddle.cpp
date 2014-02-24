#include "Paddle.h"


Paddle::Paddle(sf::Vector2f v, float speed, sf::Texture t)
{
	velocity = sf::Vector2f(0.0f, speed);
	position = v;
	pTexture = t;
	pSprite.setTexture(pTexture);
	pSprite.setPosition(position);
}

//Moce the paddle based off of the input enum
void Paddle::MovePaddle(Input i)
{
	if(CheckBounds(i))
	{
		position += velocity * (float)i;
		pSprite.setPosition(position);
	}
}

sf::FloatRect Paddle::GetSpriteBoundingBox()
{
	return pSprite.getGlobalBounds();
}

//Going to have to check the collider 
void Paddle::Update(sf::Time deltaTime)
{

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