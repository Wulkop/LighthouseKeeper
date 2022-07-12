//
// Created by jan on 12.07.22.
//

#include <set>
#include <algorithm>
#include <filesystem>
#include <fstream>
#include "ProcessCrawler.h"

#if defined(_WIN32) || defined(WIN32)
#include <Windows.h>
#include <psapi.h>
#endif
bool ProcessCrawler::isNumber(const std::string& s)
{
    return !s.empty() && std::find_if(s.begin(), s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end();
}
std::set<std::string> ProcessCrawler::GetAllRunningProcessNames()
{
    std::set<std::string> processNames;
#ifdef __unix__
    for (const auto & entry : std::filesystem::directory_iterator("/proc"))
    {
        auto path = entry.path();
        std::string name = path.filename().string();
        if(ProcessCrawler::isNumber(name))
        {
            std::filesystem::path cmdlinePath = (path.string() + "/cmdline");
            // Open the stream to 'lock' the file.
            std::ifstream f(cmdlinePath, std::ios::in | std::ios::binary);
            if(f.good() && f.is_open())
            {
                // Create a buffer.
                std::string result;
                std::getline(f, result);
                size_t splitPos = result.find('\0');
                result = result.substr(0,splitPos);

                splitPos = result.find(' ');
                if(splitPos != std::string::npos)
                {
                    result = result.substr(0,splitPos);
                }
                if(!result.empty())
                {
                    processNames.insert(std::filesystem::path(result).filename().string());
                }
            }
        }
    }
#elif defined(_WIN32) || defined(WIN32)

    // Get the list of process identifiers.

    DWORD aProcesses[1024], cbNeeded, cProcesses;
    unsigned int i;

    if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
    {
        return processNames;
    }


    // Calculate how many process identifiers were returned.

    cProcesses = cbNeeded / sizeof(DWORD);

    // Print the name and process identifier for each process.

    for (i = 0; i < cProcesses; i++)
    {
        if (aProcesses[i] != 0)
        {
            TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");

            // Get a handle to the process.

            HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, aProcesses[i]);

            // Get the process name.
            if (NULL != hProcess)
            {
                HMODULE hMod;
                DWORD cbNeeded;

                if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded))
                {
                    GetModuleBaseName(hProcess, hMod, szProcessName, sizeof(szProcessName) / sizeof(TCHAR));
                }
            }

            // Print the process name and identifier.

            processNames.insert(szProcessName);

            // Release the handle to the process.

            CloseHandle(hProcess);
        }
    }

#endif
    return processNames;
}
