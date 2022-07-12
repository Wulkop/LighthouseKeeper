#pragma once
#include <thread>
#include <SocketCommunication.h>
#include <Settings.h>

class ThreadMonitor
{
public:
	void StartThead();
	void Run();
	void EndThread();

	void MainLoopIteration(Settings& settings, SocketCommunication& com);
private:
	std::thread m_Thread;
	bool m_Stopped = false;
};