#ifndef ENGINE_H
#define ENGINE_H

#include "Paddle.h"
#include "Globals.h"
#include "Player.h"
#include "Ball.h"
#include <SFML/Graphics.hpp>

class Engine
{
public:
	Engine();
	void Update();
	void Draw();
	sf::RenderWindow* window;
	Player* player1;
	Player* player2;
	Ball* ball;
private:
	void HandleInput();
	void CheckCollision();

};

#endif