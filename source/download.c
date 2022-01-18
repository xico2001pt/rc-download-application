#include "../headers/download.h"
#include "../headers/transport.h"

#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

    if (argc != 2) ERROR("usage: download ftp://[<user>:<password>@]<host>/<url-path>");

    Arguments args;
    if (parseArguments(argv[1], &args) < 0) ERROR("main: parseArguments()");

    printf("user: %s\npassword: %s\nhost: %s\nfile_path: %s\n", args.user->data, args.password->data, args.host->data, args.file_path->data);   // DEBUG

    Buffer * ip = retrieveIpAddress(args.host);
    if (ip == NULL) ERROR("main: retrieveIpAddress()");

    FTP socket_fds;
    SocketInfo socket_info = {ip, 21};
    if ((socket_fds.control_socket_fd = connectSocket(&socket_info)) < 0) ERROR("main: connectSocket()");
    destroyBuffer(ip);
    
    Buffer * response = receiveResponse(socket_fds.control_socket_fd);
    if (response == NULL || strncmp(response->data, "220", 3) != 0) ERROR("main: Incorrect welcome message");
    destroyBuffer(response);

    if (login(socket_fds.control_socket_fd, args.user, args.password) < 0) ERROR("main: login()");

    if (enterPassiveMode(&socket_fds) < 0) ERROR("main: enterPassiveMode()");

    if (downloadFile(&socket_fds, args.file_path) < 0) ERROR("main: downloadFile()");

    destroyArguments(&args);
    
    return 0;
}

int parseArguments(const char * url_path, Arguments * args) {
    // TODO: USER E PASSWORD OPCIONAIS
    char * token;
    if ((token = strstr(url_path, "://")) == NULL || strncmp(url_path, "ftp", strlen(url_path) - strlen(token)) != 0) {
        ERROR("parseArguments(): Incorrect protocol");
    }
    token += 3;

    if ((token = strtok(token, ":")) == NULL) ERROR("parseArguments(): User invalid");

    args->user = createBuffer(token, strlen(token));

    if ((token = strtok(NULL, "@")) == NULL) ERROR("parseArguments(): Password invalid");

    args->password = createBuffer(token, strlen(token));

    if ((token = strtok(NULL, "/")) == NULL) ERROR("parseArguments(): Host name invalid");

    args->host = createBuffer(token, strlen(token));

    if ((token = strtok(NULL, "")) == NULL) ERROR("parseArguments(): Url path invalid");

    args->file_path = createBuffer(token, strlen(token));

    return 0;
}

void destroyArguments(Arguments * args) {
    destroyBuffer(args->user);
    destroyBuffer(args->password);
    destroyBuffer(args->host);
    destroyBuffer(args->file_path);
}

Buffer * retrieveIpAddress(const Buffer * host) {
    struct hostent * h;
    if ((h = gethostbyname(host->data)) == NULL) NERROR("retrieveIpAddress(): Failed to obtain ip by host name");
    char * temp = inet_ntoa(*((struct in_addr *) h->h_addr));
    Buffer * ip = createBuffer(temp, strlen(temp));
    return ip;
}

int login(int control_socket_fd, const Buffer * user, const Buffer * password) {
    
    Buffer * command = createBuffer("user ", 5);
    concatBuffers(command, user);
    int result = sendCommand(control_socket_fd, command);
    destroyBuffer(command);
    if (result < 0) ERROR("login(): User sendCommand()");

    Buffer * response = receiveResponse(control_socket_fd);
    if (response == NULL) ERROR("login(): User recieveResponse()");
    destroyBuffer(response);

    command = createBuffer("pass ", 5);
    concatBuffers(command, password);
    result = sendCommand(control_socket_fd, command);
    destroyBuffer(command);
    if (result < 0) ERROR("login(): Password sendCommand()");

    if ((response = receiveResponse(control_socket_fd)) == NULL) ERROR("login(): Password recieveResponse()");
    if (strncmp(response->data, "230", 3) != 0) ERROR("login(): User and Password Combination Failed");
    destroyBuffer(response);
    
    return 0;
}

int enterPassiveMode(FTP * ftp) {
    Buffer * command = createBuffer("pasv", 4);
    int result = sendCommand(ftp->control_socket_fd, command);
    destroyBuffer(command);
    if (result < 0) ERROR("enterPassiveMode(): Send pasv command");

    Buffer * response = receiveResponse(ftp->control_socket_fd);
    if (response == NULL || strncmp(response->data, "227", 3) != 0) ERROR("enterPassiveMode(): Recieve pasv response");

    SocketInfo data_socket_info;
    if (parsePasvResponse(response, &data_socket_info) < 0) ERROR("enterPassiveMode(): Parse data socket address error");

    destroyBuffer(response);

    int fd;
    if ((fd = connectSocket(&data_socket_info)) < 0) ERROR("enterPassiveMode(): Connect data socket error");

    ftp->data_socket_fd = fd;
    return 0;
}

long retrieveFile(int control_socket_fd, const Buffer * file_path) {
    Buffer * command = createBuffer("retr ", 5);
    concatBuffers(command, file_path);
    int result = sendCommand(control_socket_fd, command);
    destroyBuffer(command);
    if (result < 0) ERROR("retrieveFile(): Send retr command");

    Buffer * response = receiveResponse(control_socket_fd);
    if (response == NULL || strncmp(response->data, "150", 3) != 0) ERROR("retrieveFile(): Receive retr response");

    int bytes_to_read;
    if ((bytes_to_read = parseRetrResponse(response)) < 0) ERROR("retrieveFile(): Parse File Size Information error");

    destroyBuffer(response);
    return bytes_to_read;
}

int downloadFile(const FTP * ftp, const Buffer * file_path) {
    long bytes_to_read = retrieveFile(ftp->control_socket_fd, file_path);
    if (bytes_to_read < 0) ERROR("downloadFile(): Retrieve file");
    printf("File Size: %ld\n", bytes_to_read);

    Buffer * file_name = pathToFilename(file_path);
    int fd = open(file_name->data, O_WRONLY | O_CREAT, 0666);
    destroyBuffer(file_name);
    if (fd < 0) {
        close(fd);
        ERROR("downloadFile(): Create file");
    }

    Buffer * file = allocateBuffer(bytes_to_read);
    long total_bytes_read = 0, bytes_read;
    
    printf("> Progress: 0%%");
    while (total_bytes_read < file->length) {
        if ((bytes_read = read(ftp->data_socket_fd, file->data + total_bytes_read, file->length - total_bytes_read)) < 0) {
            close(fd);
            destroyBuffer(file);
            ERROR("downloadFile(): Read file");
        }
        total_bytes_read += bytes_read;
        printf("\r> Progress: %3d%%", (int) (100 * total_bytes_read / file->length));
    }
    printf("\n");

    Buffer * response = receiveResponse(ftp->control_socket_fd);
    if (response == NULL || strncmp(response->data, "226", 3) != 0) {
        close(fd);
        destroyBuffer(file);
        destroyBuffer(response);
        ERROR("downloadFile(): Retr response error");
    }
    
    destroyBuffer(response);

    if (write(fd, file->data, file->length) < file->length) {
        close(fd);
        destroyBuffer(file);
        ERROR("downloadFile(): Write file");
    }
    
    close(fd);
    destroyBuffer(file);
    return 0;
}