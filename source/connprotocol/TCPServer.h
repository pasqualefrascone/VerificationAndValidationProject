/*
 * TCPServer.h
 *
 *  Abstract class
 *      Author: pasquale
 */

#ifndef CONNPROTOCOL_TCPSERVER_H_
#define CONNPROTOCOL_TCPSERVER_H_


#include <stdio.h>
//#include <unistd.h>
#include <netinet/in.h> //strutture dati socket, cioè le socket vere e proprie
#include <sys/types.h>
#include <sys/socket.h> //domini implementativi AF

//#include <stdlib.h>

#include "../socket/ServerTCPSocket.h"
#include "ClientHandler.h"

#include <vector>
#include <iterator>

class TCPServer {

private:
	ServerTCPSocket servSock;
	std::vector<ClientHandler*> handlers;
	/*int localPort;
	unsigned int buffReaderSize;
	unsigned int buffWriterSize;
*/
public:
	TCPServer(int pendingConnectionQueueSize,int localPort,unsigned int buffReaderSize,unsigned int buffWriterSize)
	:servSock(pendingConnectionQueueSize,localPort,buffReaderSize,buffWriterSize)
	{}

	TCPServer(int localPort)
	:servSock(1,localPort,1024,1024)
	{}

	virtual ~TCPServer(){
		for (ClientHandler *c:handlers){
			delete c;
		}
	};

	/**
	 * ClientHandler based class must be instantiated inside this function in the heap. for poliymorfic behavior
	 */
	virtual ClientHandler* getClientHandler(/*ClientHandler *dest,*/BufferedSocket *bs) =0;


	virtual void start(){
		//char buffer[bufferSize];

		BufferedSocket *bs;
		ClientHandler *ch;


		while(true){  // there should be some condition that shut down the server.. TODO

		    bs= servSock.acceptCon();// blocking
		    //std::cout<<"TCPServer: accepted con. getting handler.."<<std::endl;
		    ch=getClientHandler(/*ch,*/bs);
		    //ch= new ClientHandler(bs);
		    //std::cout<<"TCPServer: accepted con. getted handler.."<<std::endl;

		    handlers.push_back(ch);
		    //std::thread t {[](ClientHandler ch){ch.manage();},std::ref(ch)};
		    //std::cout<<"TCPServer: accepted con. threading handler.."<<std::endl;
		    ch->startHandle();
		    //std::thread t(&ClientHandler::handle,ch);

		    //t.detach();
		    //std::cout<<"TCPServer: accepted con. threading handler done.."<<std::endl;
		    //t.join();

		}

	}

};

#endif /* CONNPROTOCOL_TCPSERVER_H_ */
