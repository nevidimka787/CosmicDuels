#pragma once

#include <WinSock2.h>

#define NETWORK__NULL	0
#define NETWORK__CLIENT	1
#define NETWORK__SERVER	2

/*
The function initialisate all network functions and libraries.
*/
int InitNetwork();

/*
The function starts the server powered by current program.
If server sturtup successfully
then function will write socket in out_socket_p variable
else out_socket_p variable will be not changed.
port - string that collect number of port
out_socket_p - pointer to socket in that will be wrote the socket of server
*/
int StartupServer(const char* port, SOCKET* out_socket_p);

/*
The function stop server that stay on indicated socket.
*/
int StopServer(SOCKET server_socket);

/*
The function connects program to server.
If connection successfully
then function will write socket of server to out_socket_p variable
else out_socket_p variable will be not changed.
server_name - name of the server
port - port of the server
out_socket_p - pointer to socket in that will be wrote the socket of server
*/
int ConnetToServer(const char* server_name, const char* port, SOCKET* out_socket_p);
int DisconnetFromServer(SOCKET server_socket);
int ShutdownNetwork();