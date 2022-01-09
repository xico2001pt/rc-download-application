#include "../headers/download.h"
#include "../headers/transport.h"

#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

int main(int argc, char *argv[]) {
    Arguments args;
    /*Buffer * host = createBuffer(argv[1], strlen(argv[1]));
    Buffer ip;
    if (retrieveIpAddress(host, &ip)) {
        printf("error\n");
        destroyBuffer(host);
        return -1;
    }
    printf("host: %s\nip: %s\n", argv[1], ip.data);
    destroyBuffer(host);*/

    if (parseArguments(argv[1], &args)) return -1;
    printf("user: %s\npassword: %s\nhost: %s\nfile_path: %s\n", args.user->data, args.password->data, args.host->data, args.file_path->data);
    
    // TODO: FREE BUFFERS IN ARGS

    Buffer ip;
    if (retrieveIpAddress(args.host, &ip) != 0) return -1;
    printf("ip: %s\n", ip);
    
    FTP ftp;
    if ((ftp.control_socket_fd = connectSocket(ip.data, 21)) < 0) return -1;
    printf("fd: %d\n", ftp.control_socket_fd);

    Response *response = receiveResponse(ftp.control_socket_fd);
    printf("%s\nlen: %d", response->message->data, response->message->length);
    
    return 0;
}

int parseArguments(const char * url_path, Arguments * args) {
    // TODO: USER E PASSWORD OPCIONAIS
    char * token;
    if ((token = strstr(url_path, "://")) == NULL || strncmp(url_path, "ftp", strlen(url_path) - strlen(token)) != 0) {
        perror("parseArguments(): Incorrect protocol");
        return -1;
    }
    token += 3;

    if ((token = strtok(token, ":")) == NULL) {
        perror("parseArguments(): User invalid");
        return -1;
    }

    args->user = createBuffer(token, strlen(token));

    if ((token = strtok(NULL, "@")) == NULL) {
        perror("parseArguments(): Password invalid");
        return -1;
    }

    args->password = createBuffer(token, strlen(token));

    if ((token = strtok(NULL, "/")) == NULL) {
        perror("parseArguments(): Host name invalid");
        return -1;
    }

    args->host = createBuffer(token, strlen(token));

    if ((token = strtok(NULL, "")) == NULL) {
        perror("parseArguments(): Url path invalid");
        return -1;
    }

    args->file_path = createBuffer(token, strlen(token));

    return 0;
}

int retrieveIpAddress(const Buffer * host, Buffer * ip) {
    struct hostent *h;

    if ((h = gethostbyname(host->data)) == NULL) {
        herror("gethostbyname()");
        return -1;
    }
    
    ip->data = inet_ntoa(*((struct in_addr *) h->h_addr)); // TODO: CHECK IF FREE() IS NEEDED
    ip->length = strlen(ip->data);
    return 0;
}

int login(int control_socket_fd, const Buffer * user, const Buffer * password);

int enterPassiveMode(int control_socket_fd);

int downloadFile(const FTP * ftp, const Buffer * file_path);