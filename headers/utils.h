#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>

#define ERROR(message) { printf("%s\n", message); return -1; }
#define NERROR(message) { printf("%s\n", message); return NULL; }

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

/**
 * @brief Creates a Buffer object
 * 
 * @param data      Data to be written in the buffer
 * @param length    Length of the data
 * @return Buffer*  Buffer containing the given data
 */
Buffer * createBuffer(const char *data, size_t length);

/**
 * @brief Allocates space for a buffer, but doesn't write any data
 * 
 * @param length    Length of the data
 * @return Buffer*  Buffer containing the given size of allocated space
 */
Buffer * allocateBuffer(size_t length);

/**
 * @brief Frees everything from the buffer
 * 
 * @param buffer 
 */
void destroyBuffer(Buffer *buffer);

/**
 * @brief Concatenates two buffers, holding the result in the first buffer
 * 
 * @param dest  First part of the result (and also the destination buffer)
 * @param src   Second part of the result
 */
void concatBuffers(Buffer *dest, const Buffer *src);

/**
 * @brief Parses the information given in the response by the server on pasv command
 * 
 * @param response          Response given by the server on pasv command
 * @param data_socket_info  Information retrieved
 * @return int              0 on success, -1 otherwise
 */
int parsePasvResponse(const Buffer * response, SocketInfo * data_socket_info);

/**
 * @brief Parses the number of bytes given in the response by the server on retr command
 * 
 * @param response  Response given by the server on retr command
 * @return long     Number of bytes to be read
 */
long parseRetrResponse(const Buffer * response);

/**
 * @brief Retrieves the filename from the given path
 * 
 * @param file_path Path that contains the filename
 * @return Buffer*  Filename retrieved from the path
 */
Buffer * pathToFilename(const Buffer * file_path);

#endif // UTILS_H
