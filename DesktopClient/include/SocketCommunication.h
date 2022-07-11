#pragma once


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
    int socketDescriptor = -1;
};
