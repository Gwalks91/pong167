#ifndef ENGINE_H
#define ENGINE_H

#include "Paddle.h"
#include "Globals.h"
#include "Player.h"
#include "Ball.h"
#include "Score.h"
#include <SFML/Graphics.hpp>

class Engine
{
public:
	Engine();
	~Engine();
	void StartGame();
	void EndGame(int playerID);
	void Update();
	void Draw();
	sf::RenderWindow* window;
	Player* player1;
	Player* player2;
	Ball* ball;
private:
	bool wasHit;
	void HandleInput();
	void CheckCollision();
	sf::Clock deltaClock;
	Score s;
	sf::Texture backGround;
	sf::Sprite backgroundSprite;
	bool startGame;
};

#endif