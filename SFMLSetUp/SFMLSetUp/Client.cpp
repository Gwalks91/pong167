#include "Client.h"
#include <vector>

Client::Client()
{
	connected = false;
	clientThread = nullptr;
}

Client::Client(const std::string& ipAddress, const int& port)
{
	clientThread = nullptr;
	socket = new sf::TcpSocket;
	sf::Socket::Status status = socket->connect(ipAddress, port);
	selector.add(*socket);
	
	connected = true;

	//Create the thread to always be checking for packets.5
	clientThread = new sf::Thread(&Client::receive, this);
	clientThread->launch();
}

Client::~Client()
{
	//Saftly end the thread and delete it.
	if(connected)
	{
		if(clientThread != nullptr)
		{
			clientThread->terminate();
		}
	}
}

void Client::connect(const std::string& ipAddress, const int& port)
{
	//Connect should only be called when creating a client using the
	//default constructor.  it does the same thing as connected
	//with arguments.
	if(!connected)
	{
		sf::Socket::Status status = socket->connect(ipAddress, port);
		selector.add(*socket);

		connected = true;

		//Create the thread to always be checking for packets.
		clientThread = new sf::Thread(&Client::receive, this);
		clientThread->launch();
	}
}

void Client::send(const std::string& stringToSend)
{
	sf::Packet stringPacket;
	stringPacket << stringToSend;
	
	if(socket->send(stringPacket) != sf::Socket::Done)
	{
		//ERROR
	}
}

void Client::receive()
{
	while(connected)
	{
		//If this is true then there is something to be read.
		if(selector.wait())
		{
			//Create the packet to recieve the incoming one.
			sf::Packet receivePacket;
			if(socket->receive(receivePacket) != sf::Socket::Done)
			{
				//ERROR
			}
			
			//Lock the queue.
			sf::Lock lock(*mutex);

			//Add to queue.
			clientPacketQueue.push(receivePacket);
		}
		//Mutex unlocks here.
	}
}

sf::Packet Client::getTopPacket()
{
	sf::Packet packetToReturn;

	if(!clientPacketQueue.empty())
	{
		sf::Lock lock(*mutex);
		packetToReturn = clientPacketQueue.front();
		clientPacketQueue.pop();
	}//Mutex unlocks here.

	return packetToReturn;
}