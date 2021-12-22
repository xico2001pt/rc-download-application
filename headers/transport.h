#ifndef TRANSPORT_H
#define TRANSPORT_H

/*
Data Structures:
    - struct with data and control sockets

Methods:
    - connectSocket(+ip address, +port) : socket descriptor or -1 if error
    - disconnectSocket(+ftp)
    - sendCommand(fd, header, body) : number of bytes written or -1
    - receiveCommand(fd, buffer, size): 0 or -1
    - sendAndReceiveCommand(ftp, header, body, response, responseSize, ) : 
    - login(ftp, username, password)
    - getFilePort(ftp)
    - retr
*/

#endif // TRANSPORT_H
