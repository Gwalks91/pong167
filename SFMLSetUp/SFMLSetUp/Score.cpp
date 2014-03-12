
#include "Score.h"

Score::Score()
	:player1Score(0),
	player2Score(0)
{

	font = GetFont("ALGER.ttf");
	text.setFont(font);
	text.setString(ConvertToString(player1Score) + " : " + ConvertToString(player2Score));
	text.setPosition(SCREEN_WIDTH/2, 0);
	text.setStyle(sf::Text::Bold);
	text.setCharacterSize(20);
	winner = 0;
}


Score::~Score()
{

}

void Score::ChangeScore(int id)
{
	switch(id)
	{
	case 1:
		player1Score++;
		break;
	case 2:
		player2Score++;
		break;
	}
}

void Score::Update(float elapsedTime)
{
	text.setString(ConvertToString(player1Score) + " : " + ConvertToString(player2Score));
	
	if(player1Score >= MAX_POINTS)
	{
		winner = 1;
	}
	if(player2Score >= MAX_POINTS)
	{
		winner = 2;
	}
}

void Score::Draw(sf::RenderWindow* w)
{
	w->draw(text);
}