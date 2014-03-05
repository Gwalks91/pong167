
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

sf::Font GetFont(std::string fontFile)
{
	sf::Font f;
	if(!f.loadFromFile(fontFile))
	{
		std::cout << "Font, " << fontFile << " was not loaded." << std::endl;
	}
	return f;
}

std::string ConvertToString(int num)
{
	std::string Result;          // string which will contain the result

	std::ostringstream convert;   // stream used for the conversion

	convert << num;      // insert the textual representation of 'Number' in the characters in the stream

	Result = convert.str(); // set 'Result' to the contents of the stream

	return Result;
}