//
// Created by jan on 12.07.22.
//

#include <set>
#include <algorithm>
#include <filesystem>
#include <fstream>
#include "ProcessCrawler.h"
bool ProcessCrawler::isNumber(const std::string& s)
{
    return !s.empty() && std::find_if(s.begin(), s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end();
}
std::set<std::string> ProcessCrawler::GetAllRunningProcessNames()
{
    std::set<std::string> processNames;
    for (const auto & entry : std::filesystem::directory_iterator("/proc"))
    {
        auto path = entry.path();
        std::string name = path.filename();
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
                    processNames.insert(std::filesystem::path(result).filename());
                }
            }
        }
    }
    return processNames;
}
