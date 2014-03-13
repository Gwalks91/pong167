#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <queue>

#include <SFML/Network.hpp>
#include <SFML/System.hpp>

//This is a TCP client.
class Client
{
public:
	//Creates a client but does not connect.
	Client();
	//Create a client and connects it to the ip address and port.
	Client(const std::string& ipAddress, const int& port);
	~Client();
	//Connects to the ip address and port.
	void connect(const std::string& ipAddress, const int& port);
	//Send a string.
	void send(const std::string& stringToSend);
	//Check if we have got a packet.
	void receive();
	//Use a global mutex to lock and unlock threads.
	inline void setMutex(sf::Mutex* mut)
	{ mutex = mut; }
	//Returns the top packet(oldest) and removes it from queue.
	//Returns a empty packet if top is empty.
	sf::Packet getTopPacket();
	inline bool isPacketQueueEmpty()
	{ return clientPacketQueue.empty(); }
	bool isConnected()
	{ return connected; }
private:
	//The actual socket that can send and receive packets.
	sf::TcpSocket* socket;
	//A socket manager that will tell us if the sockets have anything to read.
	sf::SocketSelector selector;
	bool connected;
	sf::Thread* clientThread;
	std::queue<sf::Packet> clientPacketQueue;
	sf::Mutex* mutex;
};

#endif