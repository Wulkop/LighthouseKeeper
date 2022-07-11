#pragma once
#include <set>
#include <string>

class ProcessCrawler
{
public:
    static std::set<std::string> GetAllRunningProcessNames();
    static bool isNumber(const std::string& s);
};
