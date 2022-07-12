//
// Created by jan on 12.07.22.
//
#include <stdio.h>
#include <cstring>
#include <set>
#include <string>
#include <iostream>

#ifdef __unix__
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h>
#include <unistd.h>
#include <iostream>
#endif




#include "SocketCommunication.h"

SocketCommunication::~SocketCommunication()
{
    disconnectSocket();
}
void SocketCommunication::connectSocket(const char *address, int port)
{
#ifdef __unix__
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

    //Set recv timeout to 3 sec
    struct timeval tv;
    tv.tv_sec = 3.0;
    tv.tv_usec = 0;
    setsockopt(socketDescriptor, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);




#elif defined(_WIN32) || defined(WIN32)

    WSADATA wsaData;
    socketDescriptor = INVALID_SOCKET;
    struct addrinfo* result = NULL, * ptr = NULL, hints;
    int iResult;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0)
    {
        std::cerr<<"WSAStartup failed with error: "<< iResult <<std::endl;
        return;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    iResult = getaddrinfo(address, std::to_string(port).c_str(), &hints, &result);
    if (iResult != 0)
    {
        std::cerr << "getaddrinfo failed with error: " << iResult << std::endl;
        WSACleanup();
    }
    // Attempt to connect to an address until one succeeds
    for (ptr = result; ptr != NULL;ptr = ptr->ai_next)
    {

        // Create a SOCKET for connecting to server
        socketDescriptor = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (socketDescriptor == INVALID_SOCKET)
        {
            std::cerr<<"socket failed with error : " << WSAGetLastError() << std::endl;
            WSACleanup();
            return;
        }

        // Connect to server.
        iResult = connect(socketDescriptor, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR)
        {
            closesocket(socketDescriptor);
            socketDescriptor = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (socketDescriptor == INVALID_SOCKET)
    {
        std::cerr<<"Unable to connect to server!"<<std::endl;
        WSACleanup();
        return;
    }

    //Set recv timeout to 3 sec
    // WINDOWS
    DWORD timeout = 3.0 * 1000;
    setsockopt(socketDescriptor, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof timeout);
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
#ifdef __unix__
    if(socketDescriptor != -1)
    {
        close(socketDescriptor);
    }
#elif defined(_WIN32) || defined(WIN32)
    // cleanup
    closesocket(socketDescriptor);
    WSACleanup();
#endif
}
