/*
 * SocketWriter.h
 *
 *      Author: pasquale
 */

#ifndef SOCKET_SOCKETWRITER_H_
#define SOCKET_SOCKETWRITER_H_

class SocketWriter {

private:
	int fileDescriptor;


public:
	SocketWriter();
	virtual ~SocketWriter();


};

#endif /* SOCKET_SOCKETWRITER_H_ */
