/*
 * CircularBuffer.h
 *
 *  a thread safe circular buffer.
 *      Author: pasquale
 */

#ifndef FIFO_CIRCULARBUFFER_H_
#define FIFO_CIRCULARBUFFER_H_
#include <stdexcept>
#include <mutex>
#include <string.h> //memcpy

//#include <fstream> //for debugging methods loga logp

class CircularBuffer {

protected:
	std::mutex *mu;
	unsigned int fifoSize;
	char *buffer;
	unsigned int oldest; //placeholder  for reader( poll, peek)
	unsigned int newest; //placeholder for writer (add)
	unsigned int occupiedBytes;

	/**
	 *  [  1 2 3 4 5 6 7 8 9 ]	 *
	 *       |         |       --> return 3 bytes
	 *     oldest    newest
	 *
	 *  [  1 2 3 4 5 6 7 8 9 ]	 *
	 *       |         |       --> return 5 bytes
	 *     newest    oldest
	 *
	 *  [  1 2 3 4 5 6 7 8 9 ]	 *
	 *                 |       --> return 0 bytes if (occupiedBytes==fifosize) else return 3 bytes.
	 *               newest
	 *               oldest
	 */
	virtual int bytesCanContiguouslyWrite() final{

		if (newest>oldest) return fifoSize-newest; //write from newest until fifosize
		else if(newest<oldest) return oldest-newest; //write from newest until oldest
		else /*if(newest==oldest)*/{
				bool full=occupiedBytes==fifoSize;
				bool empt=occupiedBytes==0;
				if (!full && !empt){
					//std::cerr<<"occupiedBytes: "<<occupiedBytes<<std::endl;
					throw std::logic_error("byteCanWriteOnFifo: invalid value of occupied bytes");
					return -1;}
				else if (full) return 0; //can't write
				else{  	// if isempty
					newest=0; oldest=0;  //set placeholders to 0
					return fifoSize; //can
				}

			}
	}

	/**
	 * same reasoning as 'bytesCanContiguouslyWrite'
	 */
	virtual int bytesCanContiguouslyRead() final{
		if (newest>oldest) return newest-oldest; //from oldest to newest
		else if(newest<oldest) return fifoSize-oldest; // from oldest to end of buffer
		else /*if(newest==oldest)*/{
			bool full=occupiedBytes==fifoSize;
			bool empt=occupiedBytes==0;
			if (!full && !empt){
				//std::cerr<<"occupiedBytes: "<<occupiedBytes<<std::endl;
				throw std::logic_error("byteCanReadOnFifo: invalid value of occupied bytes");
				return -1;}
			else if (full) return fifoSize;
			else { //if is empty
				newest=0; //set placeholders to 0
				oldest=0;
				return 0;
			}

		}
	}


public:



	CircularBuffer(unsigned int fifoSize)
	:fifoSize(fifoSize),buffer(new char[fifoSize]),oldest(0),newest(0),occupiedBytes(0)
	{
		mu=new std::mutex();
		if(fifoSize>1048576){
			throw std::invalid_argument("can't instantiate a buffer of size > 1 MB");
		}

		if(fifoSize<100){
			throw std::invalid_argument("can't instantiate a buffer of size < 100 bytes");
		}



		//pollf.open("poll.txt", std::ofstream::out | std::ofstream::trunc);
		//addf.open("add.txt", std::ofstream::out  | std::ofstream::trunc);

	}



	virtual ~CircularBuffer(){delete [] buffer;delete mu;};

	/**
	 *
	 */
	virtual bool poll(char * dest, unsigned int sizeOfPolling){


		mu->lock();
		if (occupiedBytes<sizeOfPolling){ // if there is not at least sizeOfPollingByte to read
			//logp("no byte to read. ");
			mu->unlock();
			return false;
		}
		else{
			unsigned int canReadByte=bytesCanContiguouslyRead();
			if (sizeOfPolling<=canReadByte){ // can poll without problem
				//logp("reading sizeOfPoll. ");

				memcpy(dest,&buffer[oldest],sizeOfPolling);
				oldest=(oldest%fifoSize)+sizeOfPolling;
				occupiedBytes-=sizeOfPolling;
				//std::cout<<"polled "<<*(int*)dest<<std::endl;
				//logp("readed sizeOfPoll. ");

				mu->unlock();
				return true;
			}else{

				//std::cout<<"can read1: "<<canReadByte<<std::endl;
			//	logp("reading trunc1. ");

				memcpy(dest,&buffer[oldest],canReadByte);  // read until it can

				oldest=0; // cause the remaining part is at the beginning of the buffer
				occupiedBytes-=canReadByte;
			//	logp("readed trunc1. ");

				int readedBytes=canReadByte;
				for (int i = 0; i < readedBytes; ++i) {
				//	std::cout<<std::bitset<8> (buffer[oldest+i])<<std::endl;
				}

				canReadByte=bytesCanContiguouslyRead();
				int remainingBytesToRead=sizeOfPolling-readedBytes;
				/*
				while(canReadByte<remainingBytesToRead){ // cause we use occupied byte to get freebyte -> the while guards is alwais true
					std::this_thread::yield();
					canReadByte=byteCanReadOnFifo();
				}*/

				//std::cout<<"can read2: "<<canReadByte<<std::endl;
				//logp("reading trunc2. ");

				memcpy(&dest[readedBytes],&buffer[oldest], remainingBytesToRead);
				for (int i = 0; i < remainingBytesToRead; ++i) {
				//	std::cout<<std::bitset<8> (buffer[oldest+i])<<std::endl;
				}

				oldest+=remainingBytesToRead;
				occupiedBytes-=remainingBytesToRead;
				//std::cout<<"end.  "<<*(int*)dest<<std::endl;
				//logp("readed trunc1. ");

				mu->unlock();
				return true;
			}
		}
	}

	virtual bool peek(char *dest,unsigned int sizeOfPeeking){

		//consumerLock();
		mu->lock();
		if (occupiedBytes<sizeOfPeeking){ // if there is not at least sizeOfPollingByte to read
			mu->unlock();
			return false;
		}
		else{
			unsigned int canReadByte=bytesCanContiguouslyRead();
			if (sizeOfPeeking<=canReadByte){ // can peek without problem
				memcpy(dest,&buffer[oldest],sizeOfPeeking);
				mu->unlock();
				return true;
			}else{
				memcpy(dest,&buffer[oldest],canReadByte);  // read until the end of buffer
				unsigned int readedBytes=canReadByte;
				canReadByte=bytesCanContiguouslyRead();
				unsigned int remainingBytesToRead=sizeOfPeeking-readedBytes;

				memcpy(&dest[readedBytes],&buffer[oldest], remainingBytesToRead);
				mu->unlock();
				return true;
			}
		}


	}

	virtual bool add( const char *in,unsigned int sizeOfAdding){

		mu->lock();
		unsigned int freeBytes=(fifoSize-occupiedBytes);
		if(freeBytes<sizeOfAdding){  // there is not enought free bytes to write
			//std::cout<<"FULL"<<std::endl;
			//loga("no byte for write. ");
			mu->unlock();
			return false;
		}else{
			unsigned int bytesCanW=bytesCanContiguouslyWrite();
			if(bytesCanW>=sizeOfAdding){
				//loga("writing allsize ");

				memcpy(&buffer[newest],in,sizeOfAdding);
				newest=(newest%fifoSize)+sizeOfAdding;
				occupiedBytes+=sizeOfAdding;
				//loga("writed allsize ");

				mu->unlock();
				return true;
			}else{

				//loga("writing half1 ");

				memcpy(&buffer[newest],in,bytesCanW);
				/*std::cout<< "added1 "<<bytesCanW<< " bytes"<<std::endl;
				for (int i = 0; i < bytesCanW; ++i) {
					std::cout<<std::bitset<8> (buffer[newest+i])<<std::endl;
				}
*/
				newest=0;
				occupiedBytes+=bytesCanW;
				//loga("writed half1 ");

				int writedBytes=bytesCanW;
				bytesCanW=bytesCanContiguouslyWrite();
				int remainingBytesToWrite=sizeOfAdding-writedBytes;
				//loga("writing half2 ");

				memcpy(&buffer[newest],&in[writedBytes],remainingBytesToWrite);
/*
				std::cout<< "added2 "<<remainingBytesToWrite<< " bytes"<<std::endl;
				for (int i = 0; i < remainingBytesToWrite; ++i) {
					std::cout<<std::bitset<8> (buffer[newest+i])<<std::endl;
				}
*/
				occupiedBytes+=remainingBytesToWrite;
				newest+=remainingBytesToWrite;
				//loga("writed half2 ");

				mu->unlock();
				return true;
			}

		}

	}
	/*//for debugging purpose
		void logp(std::string pref){
			using namespace std;
			std::string tow("");
			tow+=pref+" . newest: "+to_string(newest)+", oldest: "+to_string(oldest)+", occb: "+to_string(occupiedBytes);
			//pollf <<tow<<std::endl;

		}
		void loga(std::string pref){
			using namespace std;
			std::string tow("");
			tow+=pref+" . newest: "+to_string(newest)+", oldest: "+to_string(oldest)+", occb: "+to_string(occupiedBytes);
			//addf <<tow<<std::endl;

		}
	*/


};

#endif /* FIFO_CIRCULARBUFFER_H_ */
