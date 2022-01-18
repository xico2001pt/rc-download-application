#include "../headers/utils.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// Buffer

Buffer * createBuffer(const char *data, size_t length) {
    Buffer * buffer = allocateBuffer(length);
    memcpy(buffer->data, data, length);
    return buffer;
}

Buffer * allocateBuffer(size_t length) {
    Buffer * buffer = (Buffer *) malloc(sizeof(Buffer));
    buffer->data = (char *) malloc(sizeof(char) * (length + 1));
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
    if ((str = strstr(response->data, "(")) == NULL) ERROR("parsePasvResponse(): Can't find begining of IP address");
    str++;

    char * cut, address[100];
    int address_len = 0;
    for (int i = 0; i < 4; ++i) {
        if ((cut = strtok(i == 0 ? str : NULL, ",")) == NULL) ERROR("parsePasvResponse(): Invalid address");
        snprintf(address + address_len, strlen(cut) + 2, ".%s", cut);
        address_len += strlen(cut) + 1;
    }

    if ((cut = strtok(NULL, ",")) == NULL) ERROR("parsePasvResponse(): Invalid port");

    data_socket_info->port = 256 * strtol(cut, NULL, 10);

    if ((cut = strtok(NULL, ")")) == NULL) ERROR("parsePasvResponse(): Invalid port");

    data_socket_info->port += strtol(cut, NULL, 10);
    data_socket_info->address = createBuffer(address + 1, address_len - 1);
    return 0;
}

long parseRetrResponse(const Buffer * response) {
    char * str;
    if ((str = strstr(response->data, "(")) == NULL) ERROR("parseRetrResponse(): Can't find begining of File Size Information");
    str++;

    char * bytes;
    if ((bytes = strtok(str, " ")) == NULL) ERROR("parseRetrResponse(): Invalid File Size Information");

    return strtol(bytes, NULL, 10);
}

Buffer * pathToFilename(const Buffer * file_path) {
    int last_slash_idx = -1, filename_size = 0;
    for (int i = 0; i < file_path->length; ++i) {
        ++filename_size;
        if (file_path->data[i] == '/') {
            last_slash_idx = i;
            filename_size = 0;
        }
    }
    Buffer * file_name = createBuffer(file_path->data + last_slash_idx + 1, filename_size);
    return file_name;
}