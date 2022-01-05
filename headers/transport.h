#ifndef TRANSPORT_H
#define TRANSPORT_H

#include "../headers/utils.h"

/*
Data Structures:
    - struct with data and control sockets
    - response

Methods:
    x connectSocket(+ip address, +port) : socket descriptor or -1 if error
    x disconnectSocket(+ftp)
    x sendCommand(fd, command) : number of bytes written or -1
    x receiveResponse(fd, buffer): 0 or -1
    - sendAndReceiveCommand(ftp, header, body, response, responseSize, ) : 
    - login(ftp, username, password)
    - getFilePort(ftp)
    - retr
*/

/**
 * @brief Opens a socket in the given address and port
 * 
 * @param address Char array with the address of the server we want to connect the socket to (Ex.: "192.168.28.96")
 * @param port    TCP port
 * @return int socketFd > 0 on success, -1 otherwise
 */
int connectSocket(char * address, int port);

/**
 * @brief Disconnects the given socket
 * 
 * @param socketFd  File descriptor of the socket
 * @return int 0 on success, -1 otherwise
 */
int disconnectSocket(int socketFd);

/**
 * @brief Sends a command to the given socketFd
 * 
 * @param socketFd  File descriptor of the socket
 * @param command   Buffer with the command to be sent and corresponding size
 * @return int Bytes written on success, -1 otherwise
 */
int sendCommand(int socketFd, Buffer * command);

/**
 * @brief Reads all line in a response from the given socketFd
 * 
 * @param socketFd  File descriptor of the socket
 * @return Response Response obtained from the socket
 */
Response * receiveResponse(int socketFd);

#endif // TRANSPORT_H
