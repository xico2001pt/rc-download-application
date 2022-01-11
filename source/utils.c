#include "../headers/utils.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// Buffer

Buffer * createBuffer(const char *data, size_t length) {
    Buffer *buffer = (Buffer *) malloc(sizeof(Buffer));
    buffer->data = (char *) malloc(sizeof(char) * (length + 1));
    memcpy(buffer->data, data, length);
    buffer->data[length] = '\0';
    buffer->length = length;
    return buffer;
}

void destroyBuffer(Buffer *buffer) {
    free(buffer->data);
    free(buffer);
}

void concatBuffers(Buffer *dest, const Buffer *src) {
    dest->data = (char *) realloc(dest->data, sizeof(char) * (dest->length + src->length + 1));
    snprintf(dest->data + dest->length, src->length + 1, "%s", src->data);
    dest->length += src->length;
}

int parsePasvResponse(const Buffer * response, SocketInfo * data_socket_info) {
    char * str;
    if ((str = strstr(response->data, "(")) == NULL) {
        printf("parsePasvResponse(): Can't find begining of IP address\n");
        return -1;
    }
    str++;
    char *cut, address[100];
    int address_len = 0;
    for (int i = 0; i < 4; ++i) {
        if ((cut = strtok(i == 0 ? str : NULL, ",")) == NULL) {
            printf("parsePasvResponse(): Invalid address\n");
            return -1;
        }
        snprintf(address + address_len, strlen(cut) + 2, ".%s", cut);
        address_len += strlen(cut) + 1;
    }
    if ((cut = strtok(NULL, ",")) == NULL) {
        printf("parsePasvResponse(): Invalid port\n");
        return -1;
    }
    data_socket_info->port = 256 * strtol(cut, NULL, 10);
    if ((cut = strtok(NULL, ")")) == NULL) {
        printf("parsePasvResponse(): Invalid port\n");
        return -1;
    }
    data_socket_info->port += strtol(cut, NULL, 10);
    data_socket_info->address = createBuffer(address + 1, address_len - 1);
    return 0;
}