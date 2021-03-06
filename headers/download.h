#ifndef DOWNLOAD_H
#define DOWNLOAD_H

#include "../headers/utils.h"

#define MAX_BYTES_READ 4096

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

/**
 * @brief Parses the arguments given by the user as a string to a struct
 * 
 * @param url_path  User input
 * @param args      Struct containing the parsed results
 * @return int      0 on success, -1 otherwise
 */
int parseArguments(const char * url_path, Arguments * args);

/**
 * @brief Frees everything from the buffer
 * 
 * @param args Arguments to be destroyed
 */
void destroyArguments(Arguments * args);

/**
 * @brief Retrieves an IP address from the given host name
 * 
 * @param host      Host name to be used in IP retrieval
 * @return Buffer*  Buffer containing the IP address as a string in the format (XXX.XXX.XXX.XXX)
 */
Buffer * retrieveIpAddress(const Buffer * host);

/**
 * @brief High-level function responsible for loging in using the specified socket
 * 
 * @param control_socket_fd Control socket used to send commands and receive responses
 * @param user              User name used for login
 * @param password          Password used for login
 * @return int              0 on success, -1 otherwise
 */
int login(int control_socket_fd, const Buffer * user, const Buffer * password);

/**
 * @brief High-level function responsible for sending a command to enter passive mode
 * 
 * @param ftp   Struct that contains the control and data file descriptors for the FTP
 * @return int  0 on success, -1 otherwise
 */
int enterPassiveMode(FTP * ftp);

/**
 * @brief 
 * 
 * @param control_socket_fd Control socket used to send commands and receive responses
 * @param file_path         File path to be sent
 * @return long             Number of bytes to be read on success, -1 otherwise
 */
long retrieveFile(int control_socket_fd, const Buffer * file_path);

/**
 * @brief 
 * 
 * @param ftp       Struct that contains the control and data file descriptors for the FTP
 * @param file_path File path to be sent
 * @return int      0 on success, -1 otherwise
 */
int downloadFile(const FTP * ftp, const Buffer * file_path);

#endif // DOWNLOAD_H
