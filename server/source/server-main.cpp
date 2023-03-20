
//#include <iostream>

#include "loggers/PostgreLogger.h"
#include "connprotocol/TCPServer.h"
#include "MyServer.h"
using namespace std;


int main() {




	// setting up log on postgres
	const char * keywords[]={"host","port","dbname","user","password",NULL};
	const char * values[]={"pdb","5432","sistema","server","password",NULL};

	PostgreLogger *pgLogger=new PostgreLogger(1024,keywords,values);
	pgLogger->startLogLoop();



	MyServer server(5,5000,100,100,pgLogger);
	server.start();

	return 0;
}
