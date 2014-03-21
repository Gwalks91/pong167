#include "Client.h"

#include <time.h>

Client client = Client("127.0.0.1", 4455);

void clientUpdate()
{
	while(!client.isPacketQueueEmpty())
	{
		sf::Packet recievedPacket = client.getTopPacket();
		std::string stringReceived;
		recievedPacket >> stringReceived;

		//Grab the number from the string and set up parsing helpers.
		int currentPos = 0;
		int posOfNextSpace = 0;
		std::string subString = "";

		posOfNextSpace = stringReceived.find(" ");
		//Place the number in the substring.
		subString = stringReceived.substr(currentPos, posOfNextSpace);

		if(atoi(subString.c_str()) == 0)
		{
			//Start the game.  TO-DO
			
			//Sending the client time.
			std::time_t rawtime;
			std::time(&rawtime);

			sendAll("0 " + rawtime);
		}
		else if(atoi(subString.c_str()) == 1)
		{
			//Update the ball and set new velocity and accel. 
			//for dead reackoning.  TO-DO
		}
		else if(atoi(subString.c_str()) == 2)
		{
			//Update the opponent paddle and set new velocity and accel. 
			//for dead reackoning.  TO-DO
		}
		else if(atoi(subString.c_str()) == 3)
		{
			//Update the points.  TO-DO
		}
		else if(atoi(subString.c_str()) == 4)
		{
			//End the game.  State winner.  TO-DO
		}
	}
}