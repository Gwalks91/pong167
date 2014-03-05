
#ifndef SCORE_H
#define SCORE_H

#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Globals.h"
#include <time.h>

class Score
{
public:
	Score();
	~Score();
	void ChangeScore(int id);
	void Update(float elapsedTime);
	void Draw(sf::RenderWindow* w);
private:
	int player1Score;
	int player2Score;
	sf::Font font;
	sf::Text text;

};

#endif