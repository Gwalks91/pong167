#ifndef GLOBALS_H
#define GLOBALS_H

#include <iostream>
#include <sstream>
#include <string>
#include <SFML/Graphics.hpp>

//The dimensions of the Screen
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

sf::Texture LoadTexture(std::string s);
void NormalizeVector(sf::Vector2f &v);
float DistanceBetweenVectors(sf::Vector2f &v1, sf::Vector2f &v2);
sf::Font GetFont(std::string fontFile);
sf::Texture LoadTexture(std::string s);
std::string ConvertToString(int num);

extern const bool NETWORKED;

enum Input 
{
	MoveUp = -1,
	MoveDown = 1,
	NoInput = 0
};

#endif