/*
 * BufferedSocket.h
 *
 *      Author: pasquale
 */

#ifndef SOCKET_BUFFEREDSOCKET_H_
#define SOCKET_BUFFEREDSOCKET_H_

#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h> //strutture dati socket: sockaddr_in...
#include <sys/types.h>
#include <sys/socket.h> //domini implementativi AF
#include <thread>
#include <stdlib.h> //snprintf
#include <arpa/inet.h>
#include <netdb.h>

#include <string.h>
#include "BufferedSocketReader.h"
#include "BufferedSocketWriter.h"


class BufferedSocket {

private:
	BufferedSocketReader *skR;
	BufferedSocketWriter *skW;
	int toRemoteHostFileDescriptor;
	bool connected;
	bool loopingOn;
	//bool toRemoteConnected;
public:

	/**
	 * instantiating a 'connecting to' socket. Use in clientSide.
	 */
	BufferedSocket(char * hostName,unsigned short remotePort,unsigned int buffReaderSize,unsigned int buffWriterSize,bool loopingOn)
	:connected(false),loopingOn(loopingOn)
	{
		/*
		struct sockaddr_in saddr;
		struct hostent *h;
		int sockfd;
		//unsigned short port = 80;
		if((sockfd=socket(AF_INET, SOCK_STREAM, 0) < 0)) { // from back a couple slides
			throw "Error creating socket\n";
		}
		if((h=gethostbyname(hostAddress)) == NULL) { // Lookup the hostname
			throw "Unknown host\n";
		}
		//memset(&saddr, '\0', sizeof(saddr)); // zero structure out
		saddr.sin_family = AF_INET; // match the socket() call
		memcpy((char *) &saddr.sin_addr.s_addr, h->h_addr_list[0], h->h_length); // copy the address
		saddr.sin_port = htons(remotePort); // specify port to connect to

		if((toRemoteHostFileDescriptor=connect(sockfd, (struct sockaddr *) &saddr, sizeof(saddr)) < 0)) { // connect!
			throw strerror(errno);
		}*/
		//close(sockfd)

		struct addrinfo hints;
	    memset(&hints, 0, sizeof(hints));
	    hints.ai_family=AF_UNSPEC;
	    //hints.ai_socktype=SOCK_DGRAM;
	    hints.ai_protocol=IPPROTO_TCP;
		struct addrinfo *result, *rp;

		int s=getaddrinfo(hostName, std::to_string(remotePort).c_str(),&hints, &result);
	    if (s != 0) {
	        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
	        //exit(EXIT_FAILURE);
	    }

	    for (rp = result; rp != NULL; rp = rp->ai_next) {
	        toRemoteHostFileDescriptor = socket(rp->ai_family, rp->ai_socktype,
	                     rp->ai_protocol);
	        if (toRemoteHostFileDescriptor == -1)
	            continue;

	        if (connect(toRemoteHostFileDescriptor, rp->ai_addr, rp->ai_addrlen) != -1)
	            {//std::cout<<"client connected.."<<std::endl;
	        	break;}                  /* Success */

	        close(toRemoteHostFileDescriptor);
	    }

	    freeaddrinfo(result);           /* No longer needed */

	    if (rp == NULL) {               /* No address succeeded */
	        fprintf(stderr, "Could not connect\n");
	        exit(EXIT_FAILURE);
	    }


	    connected=true;
	    skR = new BufferedSocketReader(buffReaderSize,toRemoteHostFileDescriptor);
	    skW = new BufferedSocketWriter(buffWriterSize,toRemoteHostFileDescriptor);

	    if (loopingOn){
	    	std::thread tSKR (&BufferedSocketReader::readLoop,skR);  	tSKR.detach();
	    	std::thread tSKW (&BufferedSocketWriter::sendLoop,skW);   	tSKW.detach();
	    }

	}


	/**
	 * instantiating a socket. Use when host client request a connection
	 */
	BufferedSocket(/*struct sockaddr_in &remoteHostSockAddr, */int fd,unsigned int buffReaderSize,unsigned int buffWriterSize)
	:toRemoteHostFileDescriptor(fd),connected(true),loopingOn(false)
	{
	    connected=true;
	    skR = new BufferedSocketReader(buffReaderSize,toRemoteHostFileDescriptor);
	    skW = new BufferedSocketWriter(buffWriterSize,toRemoteHostFileDescriptor);

	    if(loopingOn){
	    	std::thread tSKR (&BufferedSocketReader::readLoop,skR);  	tSKR.detach();
	    	std::thread tSKW (&BufferedSocketWriter::sendLoop,skW);   	tSKW.detach();

	    }

	}



	virtual ~BufferedSocket(){delete skR;delete skW;};



	template<class T> bool addOnWriteBuffer(T & in){

		char *buf=static_cast<char*>(static_cast<void*>(&in));
		return skW->add(buf, sizeof(T));
	}

	void readSignal(){
		skR->readOnce();
	}

	void writeSignal(){
		skW->sendOnce();
	}


	template<class T> bool pollFromReadBuffer(T & dest){

		char *buf=static_cast<char*>(static_cast<void*>(&dest));
		return skR->poll(buf, sizeof(T));
	}


	void closeSocket(){
		close(toRemoteHostFileDescriptor);
	}

	void startReadLoop(){
		if(!skR->isLooping()) {    	std::thread tSKR (&BufferedSocketReader::readLoop,skR);  	tSKR.detach();}

	}

	void stopReadLoop(){
		skR->stopReadLoop();
	}

	void startWriteLoop(){
    	if(!skW->isLooping()) {std::thread tSKW (&BufferedSocketWriter::sendLoop,skW);   	tSKW.detach();}

	}

	void stopWriteLoop(){
		skW->stopSendLoop();
	}

	int whatIsLooping(){
		if(skR->isLooping() && skW->isLooping()) return 3; // binary 11
		else if(skW->isLooping()) return 2; // binary 10
		else if(skR->isLooping()) return 1; //binary 01
		else return 0;
	}

};

#endif /* SOCKET_BUFFEREDSOCKET_H_ */
