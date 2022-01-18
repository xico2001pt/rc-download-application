#include "../headers/transport.h"
#include "../headers/utils.h"
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
    server_addr.sin_port = htons(info->port);                       // Server TCP port must be network byte ordered

    // Opening the TCP socket
    if ((socketFd = socket(AF_INET, SOCK_STREAM, 0)) < 0) ERROR("connectSocket(): socket()");

    // Connecting to the server
    if (connect(socketFd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) ERROR("connectSocket(): connect()");

    return socketFd;
}

int disconnectSocket(int socketFd) {
    if (close(socketFd) < 0) ERROR("disconnectSocket(): close()");
    return 0;
}

int sendCommand(int socketFd, Buffer * command) {
    int bytes;
    printf("%s\n", command->data);
    Buffer * execute = createBuffer("\n", 1);
    concatBuffers(command, execute);
    destroyBuffer(execute);
    if ((bytes = write(socketFd, command->data, command->length) <= 0)) ERROR("sendCommand(): write()\n");
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
        fclose(file);
        if (line) free(line);
        NERROR("receiveResponse(): Error when trying to read a line");
    }

    Buffer *response = createBuffer(message, messageLen);

    printf("%s", message);  // DEBUG

    fclose(file);
    if (line) free(line);
    return response;
}