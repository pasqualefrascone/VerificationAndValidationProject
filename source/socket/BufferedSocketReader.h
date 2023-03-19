/*
 * SocketReader.h
 *
 *      Author: pasquale
 */

#ifndef SOCKET_BUFFEREDSOCKETREADER_H_
#define SOCKET_BUFFEREDSOCKETREADER_H_

#include "../fifo/CircularBuffer.h"

#include <sys/types.h> //size_t
#include <sys/socket.h>  //rev,recvfrom,recvmsg //recv blocks until any data is readable. But you don't know the size in advance
#include <poll.h>	//poll, struct pollfd
#include <thread>


class BufferedSocketReader: public CircularBuffer {

private:
	using CircularBuffer::add;
	int fileDescriptor;
	bool stopListenFlag,isLoopingFlag;
	int bytesReceived=0;



public:
	BufferedSocketReader(unsigned int buffSize,int fd)
	:CircularBuffer(buffSize),fileDescriptor(fd),stopListenFlag(false),isLoopingFlag(false)
	{}

	virtual ~BufferedSocketReader(){};




	void readLoop() {
		//unsigned int word=4;
		int bytesReceived=0;
		unsigned int bytesCanW=0;
		int pollRes=0;


		struct pollfd pStruct;
		pStruct.fd=fileDescriptor;
		pStruct.events= POLLIN;
		struct pollfd pStructs[1]={pStruct};

		isLoopingFlag=true;
		while (!stopListenFlag){
			mu->lock();
			bytesCanW=bytesCanContiguouslyWrite();
			//unsigned int freeBytes=(fifoSize-occupiedBytes);
			if(bytesCanW<1){  // there is not enought contiguos bytes to write
				//std::cout<<"FULL"<<std::endl;
				//loga("no byte for write. ");
				mu->unlock();
				continue;
			}else{//bytesCanW>0
					//loga("writing allsize ");

					pollRes=::poll(pStructs,1,1);  // perfoming a (blocking) timeout poll of max 1ms.
					if (pollRes>0){
						bytesReceived+=recv(fileDescriptor,&buffer[newest],bytesCanW,0);

						newest=(newest%fifoSize)+bytesReceived;
						occupiedBytes+=bytesReceived;
						mu->unlock();
					}
					else{
						mu->unlock();
						if(pollRes==-1){
							throw strerror(errno);  //TODO handle errors in better ways
						}
						else{
							std::this_thread::yield();
						}

					}

					continue;
			}

		}
		isLoopingFlag=false;

	}

/**
 * is blocking cause poll(...,-1)
 */
	int readOnce(){

		int bytesReceived=0;
		unsigned int bytesCanW=0;
		int pollRes=0;


		struct pollfd pStruct;
		pStruct.fd=fileDescriptor;
		pStruct.events= POLLIN;
		struct pollfd pStructs[1]={pStruct};


		mu->lock();
		bytesCanW=bytesCanContiguouslyWrite();
		//unsigned int freeBytes=(fifoSize-occupiedBytes);
		if(bytesCanW<1){  // there is not enought bytes to write
			//std::cout<<"FULL"<<std::endl;
			//loga("no byte for write. ");
			mu->unlock();
			return 0;
		}else{//bytesCanW>0
			pollRes=::poll(pStructs,1,-1); //blocking poll   //(erased comment)* perfoming a (non-blocking) timeout poll of 1ms.
			if (pollRes>0){
				bytesReceived+=recv(fileDescriptor,&buffer[newest],bytesCanW,0);

				newest=(newest%fifoSize)+bytesReceived;
				occupiedBytes+=bytesReceived;
				mu->unlock();
			}
			else{
				mu->unlock();
				if(pollRes==-1){
					throw strerror(errno);  //TODO handle errors in better ways
				}
				else{
					std::this_thread::yield();
				}

			}
				//
			return bytesReceived;
		}
	}

	void stopReadLoop(){
		stopListenFlag=true;
	}

	bool isLooping(){
		return isLoopingFlag;
	}


};

#endif /* SOCKET_BUFFEREDSOCKETREADER_H_ */
