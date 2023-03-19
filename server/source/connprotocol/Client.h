/*
 * Client.h
 *
 *	Abstract class.
 *      Author: pasquale
 */

#ifndef CONNPROTOCOL_CLIENT_H_
#define CONNPROTOCOL_CLIENT_H_
#include <iostream>
#include "../utils/Util.h"
#include "../socket/BufferedSocket.h"
#include <thread>


class Client {
protected:
	BufferedSocket *bs;

public:
	Client(char* hostName,unsigned short hostPort, unsigned int sizeOfRbuff,unsigned int sizeOfWbuff,bool looping)
	{
		bs=new BufferedSocket(hostName, hostPort, sizeOfRbuff, sizeOfWbuff, looping);
	}

	virtual ~Client(){delete bs;};


	virtual void start()=0;


};

#endif /* CONNPROTOCOL_CLIENT_H_ */
