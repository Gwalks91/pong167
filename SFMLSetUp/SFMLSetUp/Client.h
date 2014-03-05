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

//The client update stuff.

//#include "Client.h"
//
//#include <time.h>
//
//Client client = Client("127.0.0.1", 4455);
//
//void clientUpdate()
//{
//	while(!client.isPacketQueueEmpty())
//	{
//		sf::Packet recievedPacket = client.getTopPacket();
//		std::string stringReceived;
//		recievedPacket >> stringReceived;
//
//		//Grab the number from the string and set up parsing helpers.
//		int currentPos = 0;
//		int posOfNextSpace = 0;
//		std::string subString = "";
//
//		posOfNextSpace = stringReceived.find(" ");
//		//Place the number in the substring.
//		subString = stringReceived.substr(currentPos, posOfNextSpace);
//
//		if(atoi(subString.c_str()) == 0)
//		{
//			//Start the game.  TO-DO
//			
//			//Sending the client time.
//			std::time_t rawtime;
//			std::time(&rawtime);
//
//			sendAll("0 " + rawtime);
//			
//			//Start the main update loop and start message sneding and recieving.
//		}
//		else if(atoi(subString.c_str()) == 1)
//		{
//			//Update the ball and set new velocity and accel. 
//			//for dead reackoning.  TO-DO
//			
//			//Function to set balls server position and velocity and interpolate towards it.
//		}
//		else if(atoi(subString.c_str()) == 2)
//		{
//			//Update the opponent paddle and set new velocity and accel. 
//			//for dead reackoning.  TO-DO
//
//			//Function to set paddles server position and velocity and interpolate towards it.
//		}
//		else if(atoi(subString.c_str()) == 3)
//		{
//			//Update the points.  TO-DO
//			
//			//Has function.
//		}
//		else if(atoi(subString.c_str()) == 4)
//		{
//			//End the game.  State winner.  TO-DO
//
//			//Stop updating and print the winner.
//		}
//	}
//}