#include <iostream>
#include <string>
#include <exception>
#include <SFML/Graphics.hpp>
#include "Globals.h"
#include "Paddle.h"
#include "Player.h"
#include "Ball.h"
#include "Engine.h"

sf::Sprite MakeSprite(sf::Texture& texture)
{
	sf::Sprite s;
	s.setTexture(texture);
	std::cout << "Made Sprite!" << std::endl;
	return s;
}

int main()
{
	Engine* e = new Engine();
	
	while (e->window->isOpen())
    {
		e->Update();
		e->Draw();
	}

    return 0;
}

