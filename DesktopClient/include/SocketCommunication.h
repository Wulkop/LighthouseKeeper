#pragma once

#if defined(_WIN32) || defined(WIN32)

//#include <windows.h>
//#include <winsock2.h>
#include <ws2tcpip.h>

#endif
#include <string>


class SocketCommunication
{
public:
    SocketCommunication(){}
    ~SocketCommunication();

    std::string SendWithResponse(std::string message);
    void connectSocket(const char * address, int port);
    void disconnectSocket();
    bool isConnected()
    {
        return socketDescriptor != -1;
    }

private:
#ifdef __unix__
    int socketDescriptor = -1;
#elif defined(_WIN32) || defined(WIN32)
    SOCKET socketDescriptor;
#endif
};
