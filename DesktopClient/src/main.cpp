
#include <iostream>
#include <set>
#include <algorithm>
#include <fstream>

#include <chrono>
#include <thread>
#include "Settings.h"
#include "SocketCommunication.h"
#include "ProcessCrawler.h"
#include <tray.hpp>
#include <ThreadMonitor.h>
#include <WindowManager.h>




int main()
{
    ThreadMonitor threadMonitor;
    WindowManager windowManager;
    Tray::Tray tray("LighthouseKeeper", "res/icon.ico");
    tray.addEntry(Tray::Button("Exit", [&] { tray.exit(); threadMonitor.EndThread(); }));
    tray.addEntry(Tray::Toggle("Toggle console", false, std::bind(&WindowManager::OnToggleConsole, windowManager, std::placeholders::_1)));
    threadMonitor.StartThead();
    windowManager.HideConsole();
    tray.run();


}
