Client : Client.o socket.o Blockable.o
	g++ -o Client Client.o socket.o Blockable.o -pthread -l rt

Client.o : Client.cpp SharedObject.h Semaphore.h
	g++ -c Client.cpp 

Server : Server.o thread.o socket.o socketserver.o Blockable.o
	g++ -o Server Server.o thread.o socket.o socketserver.o Blockable.o -pthread -l rt

Blockable.o : Blockable.h Blockable.cpp
	g++ -c Blockable.cpp

Server.o : Server.cpp thread.h
	g++ -c Server.cpp

thread.o : thread.cpp thread.h
	g++ -c thread.cpp

socket.o : socket.cpp socket.h
	g++ -c socket.cpp

socketserver.o : socketserver.cpp socket.h socketserver.h
	g++ -c socketserver.cpp
