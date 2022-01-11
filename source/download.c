#include "../headers/download.h"
#include "../headers/transport.h"

#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

int main(int argc, char *argv[]) {
    /*Arguments args;
    Buffer * host = createBuffer(argv[1], strlen(argv[1]));
    Buffer ip;
    if (retrieveIpAddress(host, &ip)) {
        printf("error\n");
        destroyBuffer(host);
        return -1;
    }
    printf("host: %s\nip: %s\n", argv[1], ip.data);
    destroyBuffer(host);*/

    Arguments args;
    if (parseArguments(argv[1], &args)) return -1;
    printf("user: %s\npassword: %s\nhost: %s\nfile_path: %s\n", args.user->data, args.password->data, args.host->data, args.file_path->data);
    
    // TODO: FREE BUFFERS IN ARGS

    Buffer ip;
    if (retrieveIpAddress(args.host, &ip) != 0) return -1;
    printf("ip: %s\n", ip);
    
    FTP ftp;
    SocketInfo info;
    info.address = &ip;
    info.port = 21;
    if ((ftp.control_socket_fd = connectSocket(&info)) < 0) return -1;

    Buffer *response = receiveResponse(ftp.control_socket_fd);

    if (login(ftp.control_socket_fd, args.user, args.password) < 0) return -1;
    if (enterPassiveMode(&ftp) < 0) return -1;

    return 0;
}

int parseArguments(const char * url_path, Arguments * args) {
    // TODO: USER E PASSWORD OPCIONAIS
    char * token;
    if ((token = strstr(url_path, "://")) == NULL || strncmp(url_path, "ftp", strlen(url_path) - strlen(token)) != 0) {
        printf("parseArguments(): Incorrect protocol\n");
        return -1;
    }
    token += 3;

    if ((token = strtok(token, ":")) == NULL) {
        printf("parseArguments(): User invalid\n");
        return -1;
    }

    args->user = createBuffer(token, strlen(token));

    if ((token = strtok(NULL, "@")) == NULL) {
        printf("parseArguments(): Password invalid\n");
        return -1;
    }

    args->password = createBuffer(token, strlen(token));

    if ((token = strtok(NULL, "/")) == NULL) {
        printf("parseArguments(): Host name invalid\n");
        return -1;
    }

    args->host = createBuffer(token, strlen(token));

    if ((token = strtok(NULL, "")) == NULL) {
        printf("parseArguments(): Url path invalid\n");
        return -1;
    }

    args->file_path = createBuffer(token, strlen(token));

    return 0;
}

int retrieveIpAddress(const Buffer * host, Buffer * ip) {
    struct hostent *h;

    if ((h = gethostbyname(host->data)) == NULL) {
        printf("gethostbyname()\n");
        return -1;
    }
    
    ip->data = inet_ntoa(*((struct in_addr *) h->h_addr)); // TODO: CHECK IF FREE() IS NEEDED
    ip->length = strlen(ip->data);
    return 0;
}

int login(int control_socket_fd, const Buffer * user, const Buffer * password) {
    Buffer *command, *response;
    int result;
    
    command = createBuffer("user ", 5);
    concatBuffers(command, user);
    result = sendCommand(control_socket_fd, command);
    destroyBuffer(command);
    if (result < 0) {
        printf("login(): User sendCommand()\n");
        return -1;
    }

    if ((response = receiveResponse(control_socket_fd)) == NULL) {
        printf("login(): User recieveResponse()\n");
        return -1;
    }
    destroyBuffer(response);

    command = createBuffer("pass ", 5);
    concatBuffers(command, password);
    result = sendCommand(control_socket_fd, command);
    destroyBuffer(command);
    if (result < 0) {
        printf("login(): Password sendCommand()\n");
        return -1;
    }
    if ((response = receiveResponse(control_socket_fd)) == NULL) {
        printf("login(): Password recieveResponse()\n");
        return -1;
    }
    if (strncmp(response->data, "230", 3) != 0) {
        printf("login(): User and Password Combination Failed\n");
        return -1;
    }
    destroyBuffer(response);
    
    return 0;
}

int enterPassiveMode(FTP * ftp) {
    Buffer * command = createBuffer("pasv", 4);
    int result = sendCommand(ftp->control_socket_fd, command);
    destroyBuffer(command);
    if (result < 0) {
        printf("enterPassiveMode(): Send pasv command\n");
        return -1;
    }

    Buffer * response = receiveResponse(ftp->control_socket_fd);
    if (response == NULL || strncmp(response->data, "227", 3) != 0) {
        printf("enterPassiveMode(): Recieve pasv response\n");
        return -1;
    }

    SocketInfo data_socket_info;
    if (parsePasvResponse(response, &data_socket_info) < 0) {
        printf("enterPassiveMode(): Parse data socket address error\n");
        return -1;
    }

    int fd;
    if ((fd = connectSocket(&data_socket_info)) < 0) {
        printf("enterPassiveMode(): Connect data socket error\n");
        return -1;
    }

    ftp->data_socket_fd = fd;
    return 0;
}

int retrieveFile(int control_socket_fd) {

}

int downloadFile(int data_socket_fd, const Buffer * file_path) {
    
}