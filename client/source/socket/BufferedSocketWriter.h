/*
 * SocketWriter.h
 *
 *      Author: pasquale
 */

#ifndef SOCKET_BUFFEREDSOCKETWRITER_H_
#define SOCKET_BUFFEREDSOCKETWRITER_H_

#include "../fifo/CircularBuffer.h"

#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <thread>


class BufferedSocketWriter: public CircularBuffer {

private:
	virtual bool poll(char * dest, unsigned int sizeOfPolling){throw;return false;};
	virtual bool peek(char * dest, unsigned int sizeOfPolling){throw;return false;};

	int fileDescriptor;
	bool stopSendFlag;
	bool isRunning;



public:
	BufferedSocketWriter(unsigned int fifoSize,int fileDescriptor)
	:CircularBuffer(fifoSize),fileDescriptor(fileDescriptor),stopSendFlag(false),isRunning(false)
	{}
	virtual ~BufferedSocketWriter(){};

	virtual void sendLoop( ){

		unsigned int bytesCanR=0;
		//int pollRes=0;
		int bytesTrasmesmitted=0;

		while(!stopSendFlag){
			mu->lock();
			//unsigned int freeBytes=(fifoSize-occupiedBytes);
			bytesCanR=bytesCanContiguouslyRead();
			if(bytesCanR<1){  // there is not enought  bytes to read
				//std::cout<<"FULL"<<std::endl;
				//loga("no byte for write. ");
				mu->unlock();
				continue;
			}else{ //bytesCanWR >0
					//loga("writing allsize ");

			    	bytesTrasmesmitted=send(fileDescriptor,&buffer[oldest],bytesCanR,0);
					oldest+= (oldest%fifoSize)+ bytesTrasmesmitted;
					occupiedBytes-=bytesTrasmesmitted;//
					//loga("writed allsize ");

					mu->unlock();
					continue;
				}

			}
	}


	/*
	virtual void sendLoop( ){

		unsigned int bytesCanR=0;
		//int pollRes=0;
		int bytesTrasmesmitted=0;

		while(!stopSendFlag){
			mu->lock();
			//unsigned int freeBytes=(fifoSize-occupiedBytes);
			bytesCanR=bytesCanContiguouslyRead();
			if(bytesCanR<1){  // there is not enought  bytes to read
				//std::cout<<"FULL"<<std::endl;
				//loga("no byte for write. ");
				mu->unlock();
				continue;
			}else{ //bytesCanWR >0
					//loga("writing allsize ");

			    	bytesTrasmesmitted=send(fileDescriptor,&buffer[oldest],bytesCanR,0);
					oldest+= (oldest%fifoSize)+ bytesTrasmesmitted;
					occupiedBytes-=bytesTrasmesmitted;//
					//loga("writed allsize ");

					mu->unlock();
					continue;
				}

			}
	}
*/

	virtual bool sendOnce(){
		if(isRunning) throw "sendLoop is Running!";

		unsigned int bytesCanR=0;
		//int pollRes=0;
		int bytesTrasmesmitted=0;


		mu->lock();
		//unsigned int freeBytes=(fifoSize-occupiedBytes);
		bytesCanR=bytesCanContiguouslyRead();
		if(bytesCanR<1){  // there is not enought contiguos free bytes to write
			//std::cout<<"FULL"<<std::endl;
			//loga("no byte for write. ");
			mu->unlock();
			return false;
		}else{ //bytesCanWR >0
				//loga("writing allsize ");

		    	bytesTrasmesmitted=send(fileDescriptor,&buffer[oldest],bytesCanR,0);
				oldest+= (oldest%fifoSize)+ bytesTrasmesmitted;
				occupiedBytes-=bytesTrasmesmitted;//
				//loga("writed allsize ");

				mu->unlock();
				return true;
			}

	}

	bool isLooping(){return isRunning;}
	void stopSendLoop(){stopSendFlag=true;}

};

#endif /* SOCKET_BUFFEREDSOCKETWRITER_H_ */
