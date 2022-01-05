#include "../headers/transport.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

int connectSocket(char * address, int port) {
    int socketFd;
    struct sockaddr_in server_addr;

    // Server address handling
    bzero((char *) &server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(address);   // 32 bit Internet address network byte ordered
    server_addr.sin_port = htons(port);                 // Server TCP port must be network byte ordered

    // Opening the TCP socket
    if ((socketFd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("connectSocket(): socket()");
        return -1;
    }

    // Connecting to the server
    if (connect(socketFd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        perror("connectSocket(): connect()");
        return -1;
    }

    return socketFd;
}

int disconnectSocket(int socketFd) {
    if (close(socketFd) < 0) {
        perror("disconnectSocket(): close()");
        return -1;
    }
    return 0;
}

int sendCommand(int socketFd, Buffer * command) {
    int bytes;
    if ((bytes = write(socketFd, command->data, command->length) <= 0)) {
        perror("sendCommand(): write()");
        return -1;
    }
    return bytes;
}

Response * receiveResponse(int socketFd) {
    FILE * file = fdopen(socketFd, "r");

    ssize_t lineLen, messageLen = 0, bytes;
    char *line = NULL, message[500];

    while((lineLen = getline(&line, &bytes, file)) > 0) {
        snprintf(message + messageLen, lineLen, "%s", line);
        messageLen += lineLen;
    }

    Response *response = createResponse(message, messageLen);

    fclose(file);
    if (line) free(line);
    return response;
}