/*
 * ClientManager.h
 *
 *  Abstract class
 *      Author: pasquale
 */

#ifndef CONNPROTOCOL_CLIENTHANDLER_H_
#define CONNPROTOCOL_CLIENTHANDLER_H_

//#include <functional>

#include "../socket/BufferedSocket.h"



#include <iostream>

class ClientHandler {


protected:
	//PostgreLogger *pgLogger;
    BufferedSocket *clientBSock;
    bool stopFlag; //True for stop
    bool runningFlag; //isRunning
    //PostgreLogger *pgLogger;

public:
    ClientHandler(BufferedSocket *sock )
	:clientBSock(sock),stopFlag(false),runningFlag(false)
	{}
	virtual ~ClientHandler(){delete clientBSock;};

    virtual bool isRunning(){return runningFlag;};
    virtual void stop(){stopFlag=true;};

    //virtual void handle(){std::cout<<"ClientHandler:.. calling handler from base class";};
    virtual void handle() =0;


    virtual void startHandle()=0;
};

#endif /* CONNPROTOCOL_CLIENTHANDLER_H_ */
