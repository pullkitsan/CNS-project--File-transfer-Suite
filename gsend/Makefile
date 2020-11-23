all:new1

new1:gsend.o cl_socket.o
	g++ grec.o cl_socket.o -o new1 -lcrypto


gsend.o: gsend.cpp
	g++ -c gsend.cpp

cl_socket.o:cl_socket.cpp 
	g++ -c cl_socket.cpp



