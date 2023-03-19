/*
 * MyServer.h
 *
 *      Author: pasquale
 */

#ifndef UTILS_MYSERVER_H_
#define UTILS_MYSERVER_H_

#include "MyClientHandler.h"

class MyServer: public TCPServer {
private :
	PostgreLogger *pgLogger;
public:
	MyServer(int pendingConnectionQueueSize,int localPort,unsigned int buffReaderSize,unsigned int buffWriterSize)
	:TCPServer(pendingConnectionQueueSize, localPort, buffReaderSize, buffWriterSize),pgLogger(NULL) {}

	MyServer(int pendingConnectionQueueSize,int localPort,unsigned int buffReaderSize,unsigned int buffWriterSize,PostgreLogger *pgLogger)
	:TCPServer(pendingConnectionQueueSize, localPort, buffReaderSize, buffWriterSize),pgLogger(pgLogger) {}


	MyServer(int localPort) 	:TCPServer(localPort),pgLogger(NULL){}
	MyServer(int localPort,PostgreLogger *pgLogger) :TCPServer(localPort),pgLogger(pgLogger){}


	virtual ~MyServer(){if(pgLogger!=NULL)delete pgLogger;};
	virtual ClientHandler* getClientHandler(/*ClientHandler *dest,*/BufferedSocket *bs) override{
		return new MyClientHandler(bs,pgLogger);
		//return MyClientHandler(bs);
	}

};

#endif /* UTILS_MYSERVER_H_ */
