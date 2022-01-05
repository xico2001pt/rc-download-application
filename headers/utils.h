#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>

// Buffer

typedef struct {
    char *data;
    size_t length;
} Buffer;

Buffer * createBuffer(const char *data, size_t length);

void destroyBuffer(Buffer *buffer);

// Response

typedef struct {
    int status;
    Buffer *message;
} Response;

Response * createResponse(char * response, size_t length);

void destroyResponse(Response *response);

#endif // UTILS_H
