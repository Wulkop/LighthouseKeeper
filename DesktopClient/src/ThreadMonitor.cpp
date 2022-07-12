#include "ThreadMonitor.h"
#include <iostream>
#include <set>
#include "ProcessCrawler.h"


void ThreadMonitor::StartThead()
{
    m_Thread = std::thread(&ThreadMonitor::Run, this);
}
void ThreadMonitor::EndThread()
{
    std::cout << "Quit signal received. Waiting for thread to finish" << std::endl;
    m_Stopped = true;
    m_Thread.join();
}
void ThreadMonitor::Run()
{
    Settings settings = Settings::LoadFromFile();
    SocketCommunication com;
    while (!m_Stopped)
    {
        MainLoopIteration(settings, com);
        std::this_thread::sleep_until(std::chrono::system_clock::now() + std::chrono::seconds(settings.timeInterval));
    }
}
void ThreadMonitor::MainLoopIteration(Settings& settings, SocketCommunication& com)
{
    std::cout << "Checking active processes" << std::endl;
    com.connectSocket(settings.address.c_str(), settings.port);
    if (com.isConnected())
    {
        std::set<std::string> processNames = ProcessCrawler::GetAllRunningProcessNames();
        if (processNames.find(settings.steamVRProcessName) != processNames.end())
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
