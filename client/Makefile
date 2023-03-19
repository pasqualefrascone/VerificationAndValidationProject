main: Client.o main.cpp
	g++ $^ -o $@ 

Client.o: Client.h Client.cpp SocketReaderFifo.o SocketWriterFifo.o
	g++  $^ -c $@ 

AbstractFifo.o: AbstractFifo.cpp 
	g++ -c $^ -o $@ 
	
SocketReaderFifo.o: SocketReaderFifo.cpp AbstractFifo.o
	g++  $^ -c $@ 
	
SocketWriterFifo.o: SocketWriterFifo.cpp  AbstractFifo.o
	g++  $^ -c $@ 
	
.PHONY: clean dev
clean:
	rm -f *.o
dev:
	g++ AbstractFifo.cpp SocketReaderFifo.cpp SocketWriterFifo.cpp Client.cpp RealClient.cpp -o main -static
	

