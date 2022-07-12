
#include <iostream>
#include <set>
#include <algorithm>
#include <fstream>

#include <chrono>
#include <thread>
#include "Settings.h"
#include "SocketCommunication.h"
#include "ProcessCrawler.h"


void MainLoopIteration(Settings & settings, SocketCommunication & com)
{
    std::cout<<"Checking active processes"<<std::endl;
    com.connectSocket(settings.address.c_str(), settings.port);
    if(com.isConnected())
    {
        std::set<std::string> processNames = ProcessCrawler::GetAllRunningProcessNames();
        if(processNames.find(settings.steamVRProcessName) != processNames.end())
        {
            com.SendWithResponse("start\n");
        }
        else
        {
            com.SendWithResponse("stop\n");
        }
        com.disconnectSocket();
    }
}

int main()
{
    Settings settings = Settings::LoadFromFile();
    SocketCommunication com;
    while(true)
    {
        MainLoopIteration(settings, com);
        std::this_thread::sleep_until(std::chrono::system_clock::now() + std::chrono::seconds(settings.timeInterval));
    }
}
