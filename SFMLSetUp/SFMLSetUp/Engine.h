#ifndef ENGINE_H
#define ENGINE_H

#include "Paddle.h"
#include "Globals.h"
#include "Player.h"
#include "Ball.h"
#include "Score.h"
#include "Client.h"
#include <SFML/Graphics.hpp>

class Engine
{
public:
	Engine();
	~Engine();
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
	void clientUpdateThread();
	void clientSendThreadUpdate();
	sf::Clock deltaClock;
	sf::Clock sendClock;
	Score s;
	sf::Texture backGround;
	sf::Sprite backgroundSprite;
	sf::Texture winScreen;
	sf::Sprite winScreenSprite;
	bool startGame;
	int clientNumber;
	time_t start;
	double timeSinceLastSend;
	Client* client;
	sf::Mutex* clientMutex;
	sf::Thread* clientThread;
	sf::Thread* clientSendThread;
	int fps;
	float timePassedTotal;
};

#endif