/*
 * ServerSocket.h
 *
 *  Created on: 19 dic 2022
 *      Author: pasquale
 */

#ifndef SOCKET_SERVERTCPSOCKET_H_
#define SOCKET_SERVERTCPSOCKET_H_

#include <stdio.h>
#include <iostream>
//#include <unistd.h>
#include <netinet/in.h> //strutture dati socket, cioè le socket vere e proprie
#include <sys/types.h>
#include <sys/socket.h> //domini implementativi AF

#include <thread>
#include "BufferedSocket.h"


class ServerTCPSocket {
private:
	int port;
	int listeningFileDescriptor;
	int pendingConnectionQueueSize;
	unsigned int buffReaderSize;
	unsigned int buffWriterSize;
public:

	ServerTCPSocket(int pendingConnectionQueueSize,int localPort,unsigned int buffReaderSize,unsigned int buffWriterSize)
	:port(localPort),pendingConnectionQueueSize(pendingConnectionQueueSize),buffReaderSize(buffReaderSize),buffWriterSize(buffWriterSize)
	{
	    struct sockaddr_in serverSock;

	    listeningFileDescriptor=socket(AF_INET, SOCK_STREAM,0); //socket descriptor
	    if(listeningFileDescriptor<0) {throw "error socket";/*TODO*/}

	    serverSock.sin_family=AF_INET;
	    serverSock.sin_addr.s_addr=htonl(INADDR_ANY);
	    serverSock.sin_port=htons(port);
	    //setsockopt(fd,SOL_SOCKET,SO_REUSEADDR,&i,sizeof(int) );

	    //bind, associazione il socket descript e la struttura della socket
	    if(bind(listeningFileDescriptor,(struct sockaddr*) &serverSock,sizeof serverSock) <0){
	        /*errore nel bind della socket*/
	    	throw strerror(errno);//"error binding";
	    }

	    if(listen(listeningFileDescriptor,pendingConnectionQueueSize)<0){ //..
	    	throw "Error listening";
	    }

	}

	virtual ~ServerTCPSocket(){};

	BufferedSocket* acceptCon(){
		struct sockaddr_in cSock;
		socklen_t cSock_len=sizeof cSock;
		//std::cout<<"ServerTCPSocket: accepting con..."<<std::endl;
		int fd= accept(listeningFileDescriptor, (sockaddr *)&cSock, &cSock_len);
		//std::cout<<"ServerTCPSocket: accepted con on fd:"<<fd<<std::endl;

		BufferedSocket *tor= new BufferedSocket(fd, buffReaderSize, buffWriterSize); // will be deleted in ClientHandler
		return tor;
	}

};

#endif /* SOCKET_SERVERTCPSOCKET_H_ */
