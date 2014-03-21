#include "Server.h"

#include <iterator>
#include <sstream>

//TESTING
#include <iostream>

Server::Server()
{
	connected = false;
	gameStarted = false;
	connectionsThread = nullptr;
	receiveThread = nullptr;
}

Server::Server(const int& port)
{
	listener = new sf::TcpListener();

	if(listener->listen(port) != sf::Socket::Done)
	{
		//ERROR
	}

	selector.add(*listener);

	connected = true;
	gameStarted = false;

	connectionsThread = nullptr;
	receiveThread = nullptr;

	connectionsThread = new sf::Thread(&Server::connectionReciever, this);
	connectionsThread->launch();

	std::cout << "Connection started and listening for clients." << std::endl;
}

Server::~Server()
{
	for(std::list<sf::TcpSocket*>::iterator it = listOfClients.begin(); it != listOfClients.end(); ++it)
	{
		sf::TcpSocket* socket = *it;
		delete socket;
	}

	listOfClients.clear();

	if(connectionsThread != nullptr)
	{
		connectionsThread->terminate();
	}
	if(receiveThread != nullptr)
	{
		receiveThread->terminate();
	}
	delete connectionsThread;
	delete receiveThread;

	std::cout << "Server cleaned up." << std::endl;
}

void Server::connect(const int& port)
{
	if(!connected)
	{
		listener = new sf::TcpListener();

		if (listener->listen(port) != sf::Socket::Done)
		{
			//ERROR
		}

		selector.add(*listener);

		connected = true;

		connectionsThread = new sf::Thread(&Server::connectionReciever, this);
		connectionsThread->launch();

		std::cout << "Connection started and listening for clients." << std::endl;
	}
}

void Server::connectionReciever()
{
	while(listOfClients.size() < MAX_PLAYERS)
	{
		//Turns true if the a socket in the celector has data.
		if(selector.wait())
		{
			//Check to see if the listener is the one with data.
			if(selector.isReady(*listener))
			{
				sf::Lock lock(*mutex);
				//This block of code will hang until a client is recieved.
				sf::TcpSocket* client = new sf::TcpSocket;
				//Accept the connection if possible.
				if (listener->accept(*client) != sf::Socket::Done)
				{
					//ERROR
					//Delete the client.
					delete client;
				}
				else
				{
					//Add the client to the cient list.
					listOfClients.push_back(client);
					selector.add(*client);

					std::cout << "Got a client." << std::endl;

					//Start getting packets if we have not already.  We start sending if
					//both clients are connected.
					if(listOfClients.size() == MAX_PLAYERS)
					{
						receiveThread = new sf::Thread(&Server::recieve, this);
						receiveThread->launch();

						gameStarted = true;
						//Calculate the current time.
						std::time_t rawtime;
						std::time(&rawtime);
						
						//Send the start game packet.
						firstSentPacketTime = rawtime;
						std::stringstream timeStringStream;
						timeStringStream << "0 1 " << rawtime;
						send(timeStringStream.str(), 1);
						timeStringStream.str("");
						timeStringStream << "0 2 " << rawtime;
						send(timeStringStream.str(), 2);

						std::cout << "Time of first packet: " << firstSentPacketTime << std::endl;

						std::cout << "Game Started.  Sent start game command." << std::endl;
					}
					//Mutex unlocks here.
				}
			}
		}
	}
	connectionsThread->terminate();
}

void Server::sendAll(const std::string& stringToSend)
{
	send(stringToSend, 1);
	send(stringToSend, 2);
	//std::cout << "Sending " << stringToSend << " to all." << std::endl;
}

void Server::send(const std::string& stringToSend, const int& clientNumber)
{
	std::list<sf::TcpSocket*>::iterator it = listOfClients.begin();
	std::advance(it, clientNumber - 1);
	sf::TcpSocket& socket = **it;

	sf::Packet packet;
	packet << stringToSend;
	socket.send(packet);

	//std::cout << "Sending: " << stringToSend << "to Client#" << clientNumber << std::endl;
}

void Server::recieve()
{
	while(!listOfClients.empty())
	{
		//If this is true then there is something to be read.
		if(selector.wait())
		{
			for(std::list<sf::TcpSocket*>::iterator it = listOfClients.begin(); it != listOfClients.end(); ++it)
			{
				sf::TcpSocket& socket = **it;

				if(selector.isReady(socket))
				{
					//Create the packet to recieve the incoming one.
					sf::Packet receivePacket;
					if(socket.receive(receivePacket) != sf::Socket::Done)
					{
						//ERROR
					}
					else
					{
						//Lock the queue.
						sf::Lock lock(*mutex);

						//Add to queue.
						serverPacketQueue.push(receivePacket);
					}
				}
			}
		}
		//Mutex unlocks here.
	}
}

sf::Packet Server::getTopPacket()
{
	sf::Packet packetToReturn;

	if(!serverPacketQueue.empty())
	{
		sf::Lock lock(*mutex);
		packetToReturn = serverPacketQueue.front();
		serverPacketQueue.pop();
	}//Mutex unlocks here.
	return packetToReturn;
}

std::list<int> Server::getDisconnectedClients()
{
	std::list<int> listOfDisClients;
	int currentClient = 1;
	for(std::list<sf::TcpSocket*>::iterator it = listOfClients.begin(); it != listOfClients.end(); ++it)
	{
		sf::TcpSocket& socket = **it;

		//If the socket is not connected then the port is 0.
		if(socket.getRemotePort() == 0)
		{
			listOfDisClients.push_back(currentClient);

			std::cout << "Client #" + currentClient << " disconnected." << std::endl;
		}

		currentClient++;
	}

	return listOfDisClients;
}