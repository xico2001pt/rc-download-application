#ifndef TRANSPORT_H
#define TRANSPORT_H

#include "../headers/utils.h"

/*
Data Structures:
    - struct with data and control sockets
    - response

Methods:
    - connectSocket(+ip address, +port) : socket descriptor or -1 if error
    x disconnectSocket(+ftp)
    - sendCommand(fd, command) : number of bytes written or -1
    - receiveCommand(fd, buffer): 0 or -1
    - sendAndReceiveCommand(ftp, header, body, response, responseSize, ) : 
    - login(ftp, username, password)
    - getFilePort(ftp)
    - retr
*/

/**
 * @brief Disconnects the given socket
 * 
 * @param socketFd  File descriptor of the socket
 * @return int 0 on success, -1 otherwise
 */
int disconnectSocket(int socketFd);

/**
 * @brief Reads all line in a response from the given socketFd
 * 
 * @param socketFd  File descriptor of the socket
 * @return Response Response obtained from the socket
 */
Response * receiveResponse(int socketFd);

#endif // TRANSPORT_H
