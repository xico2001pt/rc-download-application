#include "../headers/transport.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

int connectSocket(SocketInfo * info) {
    int socketFd;
    struct sockaddr_in server_addr;

    // Server address handling
    bzero((char *) &server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(info->address->data);   // 32 bit Internet address network byte ordered
    server_addr.sin_port = htons(info->port);                 // Server TCP port must be network byte ordered

    // Opening the TCP socket
    if ((socketFd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("connectSocket(): socket()\n");
        return -1;
    }

    // Connecting to the server
    if (connect(socketFd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        printf("connectSocket(): connect()\n");
        return -1;
    }

    return socketFd;
}

int disconnectSocket(int socketFd) {
    if (close(socketFd) < 0) {
        printf("disconnectSocket(): close()\n");
        return -1;
    }
    return 0;
}

int sendCommand(int socketFd, Buffer * command) {
    int bytes;
    Buffer * execute = createBuffer("\n", 1);
    concatBuffers(command, execute);
    destroyBuffer(execute);
    if ((bytes = write(socketFd, command->data, command->length) <= 0)) {
        printf("sendCommand(): write()\n");
        return -1;
    }
    return bytes;
}

Buffer * receiveResponse(int socketFd) {
    FILE * file = fdopen(dup(socketFd), "r");

    size_t lineLen, messageLen = 0, bytes;
    char *line = NULL, message[500];

    while((lineLen = getline(&line, &bytes, file)) > 0) {
        snprintf(message + messageLen, lineLen + 1, "%s", line);
        messageLen += lineLen;
        if (line[3] != '-') break;
    }

    if (lineLen < 0) {
        // processar erro
        return NULL;
    }

    Buffer *response = createBuffer(message, messageLen);

    printf("%s", message);

    fclose(file);
    if (line) free(line);
    return response;
}