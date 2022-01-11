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