/*
 * SocketReader.h
 *
 *      Author: pasquale
 */

#ifndef SOCKET_SOCKETREADER_H_
#define SOCKET_SOCKETREADER_H_

class SocketReader {

private:
	int fileDescriptor;


public:
	SocketReader();
	virtual ~SocketReader();
};

#endif /* SOCKET_SOCKETREADER_H_ */
