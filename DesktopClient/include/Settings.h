#pragma once


#include <string>

struct Settings
{
public:

    static Settings LoadFromFile();

    std::string steamVRProcessName = "vrserver";
    std::string address = "127.0.0.1";
    int port = 8888;
    int timeInterval = 5;

};


