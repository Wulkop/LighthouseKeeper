cmake_minimum_required(VERSION 3.13)  # CMake version check
project(LighthouseKeeper)               # Create project "simple_example"
set(CMAKE_CXX_STANDARD 17)            # Enable c++14 standard

# Add main.cpp file of project root directory as source file
set(SOURCE_FILES src/main.cpp
        src/Settings.cpp
        src/SocketCommunication.cpp
        src/ProcessCrawler.cpp)

include_directories("Dependencies/JSON")
include_directories("include")

# Add executable target with source files listed in SOURCE_FILES variable
add_executable(LighthouseKeeper ${SOURCE_FILES})
if (UNIX)
    target_link_libraries(LighthouseKeeper stdc++fs)
endif (UNIX)
if (WIN32)
    target_link_libraries(LighthouseKeeper Ws2_32 Mswsock Advapi32)
endif (WIN32)


