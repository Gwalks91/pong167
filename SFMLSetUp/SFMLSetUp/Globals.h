#ifndef GLOBALS_H
#define GLOBALS_H

#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>

//The dimensions of the Screen
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

sf::Texture LoadTexture(std::string s);
void NormalizeVector(sf::Vector2f &v);

enum Input 
{
	MoveUp = -1,
	MoveDown = 1,
	NoInput = 0
};

#endif