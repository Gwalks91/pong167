#include "Server.h"

#include <iterator>

Server::Server()
{
	connected = false;
}

Server::Server(const int& port)
{
	if (listener.listen(port) != sf::Socket::Done)
	{
		//ERROR
	}

	selector.add(listener);

	connected = true;

	connectionsThread = new sf::Thread(&Server::connectionReciever, this);
	connectionsThread->launch();
}

Server::~Server()
{
	for(std::list<sf::TcpSocket*>::iterator it = listOfClients.begin(); it != listOfClients.end(); ++it)
	{
		sf::TcpSocket* socket = *it;
		delete socket;
	}

	listOfClients.clear();

	connectionsThread->terminate();
	receiveThread->terminate();
	delete connectionsThread;
	delete receiveThread;
}

void Server::connect(const int& port)
{
	if(!connected)
	{
		if (listener.listen(port) != sf::Socket::Done)
		{
			//ERROR
		}

		selector.add(listener);

		connected = true;

		connectionsThread = new sf::Thread(&Server::connectionReciever, this);
		connectionsThread->launch();
	}
}

void Server::connectionReciever()
{
	//Turns true if the a socket in the celector has data.
	if(selector.wait())
	{
		//Check to see if the listener is the one with data.
		if(selector.isReady(listener))
		{
			//This block of code will hang until a client is recieved.
			sf::TcpSocket* client = new sf::TcpSocket;
			//Accept the connection if possible.
			if (listener.accept(*client) != sf::Socket::Done)
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

				//Start getting packets if we have not already.  We start sending if
				//both clients are connected.
				if(listOfClients.size() == MAX_PLAYERS)
				{
					receiveThread = new sf::Thread(&Server::recieve, this);
					receiveThread->launch();
				}
				//Mutex unlocks here.
			}
		}
	}
}

void Server::sendAll(const std::string& stringToSend)
{
	sf::Packet packet;
	packet << stringToSend;
	
	for(std::list<sf::TcpSocket*>::iterator it = listOfClients.begin(); it != listOfClients.end(); ++it)
	{	
		sf::TcpSocket& socket = **it;
		socket.send(packet);
	}
}

void Server::send(const std::string& stringToSend, const int& clientNumber)
{
	std::list<sf::TcpSocket*>::iterator it = listOfClients.begin();
	std::advance(it, clientNumber);
	sf::TcpSocket& socket = **it;

	sf::Packet packet;
	packet << stringToSend;
	socket.send(packet);
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