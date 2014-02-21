#ifndef PLAYER_H
#define PLAYER_H


#include "Paddle.h"
#include "Globals.h"
#include <SFML/Graphics.hpp>

class Player
{
public:
	sf::Keyboard::Key moveUp;
	sf::Keyboard::Key moveDown;
	Player(int pID, sf::Vector2f paddlePos, sf::Keyboard::Key up, sf::Keyboard::Key down);
	int GetID();
	Paddle* GetPaddle();
	void HandleInput(sf::Event e);
	void Update();
	void Draw(sf::RenderWindow* w);
private:
	int playerID;
	int score;
	sf::Font font;
	sf::Text text;
	Paddle* p;
	int GetScore();
	sf::Font GetFont(std::string fontFile);
	sf::Texture LoadTexture(std::string s);
};

#endif