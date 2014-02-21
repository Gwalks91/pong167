
#include "Globals.h"

const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 550;

sf::Texture LoadTexture(std::string s)
{
	sf::Texture t;
	if(!t.loadFromFile(s))
	{
		std::cout << "An Error has occured!" << std::endl;
	}
	return t;
}

void NormalizeVector(sf::Vector2f &v)
{
	float length = std::sqrt(std::pow(v.x, 2) * std::pow(v.y, 2));
	v.x = v.x/length;
	v.y = v.y/length;
}