#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>

// Buffer

/**
 * @brief Struct that contains a string and its length. For security purposes, the size of the string is length + 1, in order to include the null byte
 */
typedef struct {
    char *data;
    size_t length;
} Buffer;

/**
 * @brief Struct that hold an address and a port number
 */
typedef struct {
    Buffer  *address;
    int     port;
} SocketInfo;

Buffer * createBuffer(const char *data, size_t length);

void destroyBuffer(Buffer *buffer);

void concatBuffers(Buffer *dest, const Buffer *src);

int parsePasvResponse(const Buffer * response, SocketInfo * data_socket_info);

#endif // UTILS_H
