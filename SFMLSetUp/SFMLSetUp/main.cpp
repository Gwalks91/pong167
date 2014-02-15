#include <iostream>
#include <string>
#include <exception>
#include <SFML/Graphics.hpp>
#include "Globals.h"
#include "Paddle.h"
#include "Player.h"

sf::Sprite MakeSprite(sf::Texture& texture)
{
	sf::Sprite s;
	s.setTexture(texture);
	std::cout << "Made Sprite!" << std::endl;
	return s;
}

int main()
{
    sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode(1000, 550), "SFML works!");

	Player p1(sf::Vector2f(10.0f, 0.0f), sf::Keyboard::W, sf::Keyboard::S);
	Player p2(sf::Vector2f(970.0f, 0.0f), sf::Keyboard::I, sf::Keyboard::K);

    while (window->isOpen())
    {
        sf::Event event;
        while (window->pollEvent(event))
		{
			// check the type of the event...
			switch (event.type)
			{
				// window closed
				case sf::Event::Closed:
					window->close();
					break;

				// key pressed
				case sf::Event::KeyPressed:
					if (event.key.code == sf::Keyboard::Escape)
					{
						std::cout << "the escape key was pressed" << std::endl;
					}
					p1.HandleInput(event);
					p2.HandleInput(event);
					break;
				// we don't process other types of events
				default:
					break;
			}
		}

		p1.Update();
		p2.Update();

        window->clear();
		p1.Draw(window);
		p2.Draw(window);
        window->display();
    }

    return 0;
}

