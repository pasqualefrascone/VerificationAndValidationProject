/*
 * PostgreLogger.h
 *  This is a thread safe logger.
 *      Author: pasquale
 */

#ifndef POSTGRELOGGER_H_
#define POSTGRELOGGER_H_

#include <stdexcept>
#include <thread>
//#include "postgresql/libpq-fe.h"
#include "../postgresql/libpq-fe.h"
#include "../loggers/PGcommand.h"
#include "../fifo/CircularBuffer.h"

//#include <mutex>  // alredy included in circularBuffer
//#include <iostream> // for debugging

class PostgreLogger: public CircularBuffer {

private:

	using CircularBuffer::poll;
	using CircularBuffer::peek;





protected:
	PGconn *conn;
	bool stopFlag;
	bool isLooping;

	virtual void logOnPostGres()
	{
		isLooping=true;

		//std::cout<<"PostgreLogger::logOnPostGres iniziato"<<std::endl;
		ExecStatusType est;
		char dest[sizeof(PGcommand)];

		while(stopFlag!=true){

			if(poll(dest, sizeof(PGcommand)) ==false){
				std::this_thread::yield();
			}else{
				PGcommand *pgcom=static_cast<PGcommand*>(static_cast<void*>(dest) ) ;
				PGresult *res=PQexecParams(conn, pgcom->command , pgcom->paramNum, pgcom->oids, pgcom->paramValues, NULL, NULL, 0);
				if (/*ExecStatusType*/ (est=PQresultStatus(res) )!= PGRES_COMMAND_OK)
				{
				   	std::cerr<<"Insert failed: "<<PQresStatus(est)<<std::endl;
				   	pgcom->printToStdOut();
				}
				//memset(dest,0,sizeof(PGcommand));
			}
		}
		isLooping=false;
		std::cout<<"PostgreLogger::logOnPostGres finito"<<std::endl;
	}



public:
	PostgreLogger(unsigned int fifoSize,PGconn &conn)
	: CircularBuffer(fifoSize),conn(&conn),isLooping(false)
	{
		//prodFlag=false;
		//consFlag=false;
		//turno=0;//1 per producer 2 per consumer
		stopFlag=false;


	}

	PostgreLogger(unsigned int fifoSize, const char * keywords[],const char * values[])
	: CircularBuffer(fifoSize),isLooping(false)
	{
		//prodFlag=false;
		//consFlag=false;
		//turno=0;//1 per producer 2 per consumer
		stopFlag=false;

		conn=PQconnectdbParams(keywords, values, 1);

		if (PQstatus(this->conn)!=CONNECTION_OK){
			
			throw std::runtime_error("PostgreLogger: connection with postgres dbms failed!");
		}

	}


	virtual ~PostgreLogger(){};

	virtual void startLogLoop()
	{
		if(isLooping) return;
		stopFlag=false;
		std::thread t(&PostgreLogger::logOnPostGres,this);
		t.detach();
	}

	virtual void stopLogLoop(){
		stopFlag=true;
	}

};

#endif /* POSTGRELOGGER_H_ */
