#include "../headers/transport.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int disconnectSocket(int socketFd) {
    if (close(socketFd) < 0) {
        perror("close()");
        return -1;
    }
    return 0;
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