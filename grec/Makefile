all:new1

new1:grec.o srv_socket.o
	g++ grec.o srv_socket.o -o new1 -lcrypto


grec.o: grec.cpp
	g++ -c grec.cpp

srv_socket.o:srv_socket.cpp 
	g++ -c srv_socket.cpp



