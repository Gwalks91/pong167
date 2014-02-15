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
	position += velocity * (float)i;
	pSprite.setPosition(position);
}

//Going to have to check the collider 
void Paddle::Update()
{

}


void Paddle::Draw(sf::RenderWindow* w)
{
	w->draw(pSprite);

}