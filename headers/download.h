#ifndef DOWNLOAD_H
#define DOWNLOAD_H

#include "../headers/utils.h"

/**
 * @brief Struct that keeps the data from the URL argument
 */
typedef struct {
    Buffer *user;       // Username
    Buffer *password;   // Password
    Buffer *host;       // Host name
    Buffer *file_path;  // File path 
} Arguments;

/**
 * @brief Struct that contains the control and data file descriptors for the FTP
 */
typedef struct {
    int control_socket_fd;  // File descriptor of control socket
    int data_socket_fd;     // File descriptor of data socket
} FTP;

/*
Data Structures:
    x struct with arguments
    x struct with data and control sockets

Methods:
    - parseArguments(+string with user input, -struct with args) : 0 if success, otherwise 1
    - retrieveIpAddress(+host, -ip) : 0 if success, otherwise 1
    - login(ftp, username, password)
    - enterPassiveMode()
    - downloadFile(ftp, filepath)
*/

int parseArguments(const char * url_path, Arguments * args);

int retrieveIpAddress(const Buffer * host, Buffer * ip);

int login(int control_socket_fd, const Buffer * user, const Buffer * password);

int enterPassiveMode(FTP * ftp);

int retrieveFile(int control_socket_fd);

int downloadFile(int data_socket_fd, const Buffer * file_path);

#endif // DOWNLOAD_H
