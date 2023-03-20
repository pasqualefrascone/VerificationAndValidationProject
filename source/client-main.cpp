
//#include <iostream>

#include "loggers/PostgreLogger.h"
#include "MyClient.h"
using namespace std;


int main() {




	// setting up log on postgres
	const char * keywords[]={"host","port","dbname","user","password",NULL};
	const char * values[]={"progetto_frascone-postgres-1","5432","sistema","client","password",NULL};

	PostgreLogger *pgLogger=new PostgreLogger(1024,keywords,values);
	pgLogger->startLogLoop();



	MyClient client ("server1", 5000, 100,100,false,pgLogger);

	client.start();


	return 0;
}
