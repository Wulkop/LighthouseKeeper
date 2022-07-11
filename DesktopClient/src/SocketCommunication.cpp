//
// Created by jan on 12.07.22.
//
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <strings.h>
#include <cstring>
#include <set>
#include <unistd.h>

#include "SocketCommunication.h"

SocketCommunication::~SocketCommunication()
{
    disconnectSocket();
}
void SocketCommunication::connectSocket(const char *address, int port)
{
    struct hostent* host = gethostbyname(address);
    sockaddr_in sendSockAddr;
    bzero((char*)&sendSockAddr, sizeof(sendSockAddr));
    sendSockAddr.sin_family = AF_INET;
    sendSockAddr.sin_addr.s_addr =
            inet_addr(inet_ntoa(*(struct in_addr*)*host->h_addr_list));
    sendSockAddr.sin_port = htons(port);
    socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    //try to connect...
    int status = connect(socketDescriptor, (sockaddr*) &sendSockAddr, sizeof(sendSockAddr));
    if(status < 0)
    {
        std::cerr<<"Error connecting to socket!"<<std::endl;
        socketDescriptor = -1;
        return;
    }
    std::cout << "Connected to the server!" << std::endl;
#ifdef __unix__
    struct timeval tv;
    tv.tv_sec = 4.0;
    tv.tv_usec = 0;
    setsockopt(socketDescriptor, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);

#elif defined(_WIN32) || defined(WIN32)
    // WINDOWS
    DWORD timeout = timeout_in_seconds * 1000;
    setsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof timeout);
#endif

}
std::string SocketCommunication::SendWithResponse(std::string message)
{
    //create a message buffer
    char msg[1500];
    memset(&msg, 0, sizeof(msg));//clear the buffer
    strcpy(msg, message.c_str());
    int sendNum = send(socketDescriptor, (char*)&msg, strlen(msg), 0);
    std::cout << "Awaiting server response..." << std::endl;
    memset(&msg, 0, sizeof(msg));//clear the buffer
    recv(socketDescriptor, (char*)&msg, sizeof(msg), 0);
    std::cout << "Server: " << msg << std::endl;
    return std::string(msg);
}
void SocketCommunication::disconnectSocket()
{
    if(socketDescriptor != -1)
    {
        close(socketDescriptor);
    }
}
