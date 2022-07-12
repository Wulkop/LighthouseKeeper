//
// Created by jan on 12.07.22.
//

#include "Settings.h"
#include <json.hpp>
#include <fstream>
#include <iostream>
#include <filesystem>

// for convenience
using json = nlohmann::json;

Settings Settings::LoadFromFile()
{
    Settings settings;
    std::string path = std::filesystem::current_path().string() + "/settings.json";
    if(std::filesystem::exists(path))
    {
        std::ifstream settingsFile(path, std::ios::in | std::ios::binary);
        if(settingsFile.good() && settingsFile.is_open())
        {
            // Obtain the size of the file.
            const auto sz = std::filesystem::file_size(path);

            // Create a buffer.
            std::string result(sz, '\0');

            // Read the whole file into the buffer.
            settingsFile.read(result.data(), sz);

            auto j3 = json::parse(result);
            settings.address = j3["address"];
            settings.port = j3["port"];
            settings.timeInterval = j3["timeIntervalSeconds"];
            settings.steamVRProcessName = j3["processName"];

        }
        else
        {
            std::cerr<<"Could not open settings file. Using default parameters."<<std::endl;
        }
    }
    else
    {
        std::cerr<<"Expecting settings file at " << path << " but Settings file does not exist. Using default parameters."<<std::endl;
    }
    return settings;
}
