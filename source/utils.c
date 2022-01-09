#include "../headers/utils.h"
#include <string.h>
#include <stdlib.h>

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

// Response

Response * createResponse(char * response, size_t length) {
    // TODO: RETREIVE STATUS FROM RESPONSE AND ISOLATE MESSAGE
    Response *result = (Response *) malloc(sizeof(Response));
    // get status from response
    result->status = 200;
    result->message = createBuffer(response, length);
    return result;
}

void destroyResponse(Response *response) {
    destroyBuffer(response->message);
    free(response);
}