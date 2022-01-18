#ifndef TRANSPORT_H
#define TRANSPORT_H

#include "../headers/utils.h"

/**
 * @brief Opens a socket in the given address and port
 * 
 * @param info  SocketInfo struct with the address of the server we want to connect the socket to (Ex.: "192.168.28.96") and the TCP port
 * @return int socketFd > 0 on success, -1 otherwise
 */
int connectSocket(SocketInfo * info);

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
 * @return Buffer Response obtained from the socket
 */
Buffer * receiveResponse(int socketFd);

#endif // TRANSPORT_H
