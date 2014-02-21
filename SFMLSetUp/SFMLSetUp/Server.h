#pragma once
#ifndef SERVER_H
#define SERVER_H

#include <queue>
#include <list>

#include <SFML/Network.hpp>
#include <SFML/System.hpp>

class Server
{
public:
	//Create a Server with no port number.
	Server();
	//Create a Server for the port number.
	Server(const int& port);
	~Server();
	//Bind the server to a port.
	void connect(const int& port);
	//Listens for client connections.
	void connectionReciever();
	//Sends a string as a packet to all clints.
	void sendAll(const std::string& stringToSend);
	//Sends a string as a packet to a specific client.  Client ordered
	//by connection order(based zero).
	void send(const std::string& stringToSend, const int& clientNumber);
	//Recieves the packets from all clients.
	void recieve();
	void setMutex(sf::Mutex* mut)
	{ mutex = mut; }
	//Returns the top packet(oldest) and removes it from queue.
	//Returns a empty packet if top is empty.
	sf::Packet getTopPacket();
	bool isPacketQueueEmpty()
	{ return serverPacketQueue.empty(); }
private:
	//Listens for connections.
	sf::TcpListener listener;
	//Manages all the sockets of the clients.
	sf::SocketSelector selector;
	//Holds all the clients.
	std::list<sf::TcpSocket*> listOfClients;
	//Queue for packets.
	std::queue<sf::Packet> serverPacketQueue;
	bool connected;
	//The threads to listen for connections and sent messages.
	sf::Thread* connectionsThread;
	sf::Thread* receiveThread;
	sf::Mutex* mutex;

	static const int MAX_PLAYERS = 2;
};

#endif