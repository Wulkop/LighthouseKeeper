cmake_minimum_required(VERSION 3.13)  # CMake version check
project(LighthouseKeeper)               # Create project "simple_example"
set(CMAKE_CXX_STANDARD 17)            # Enable c++17 standard

add_subdirectory("Dependencies/traypp")
include_directories("Dependencies/traypp/tray/include")

# Add main.cpp file of project root directory as source file
set(SOURCE_FILES src/main.cpp
        src/Settings.cpp
        src/SocketCommunication.cpp
        src/ProcessCrawler.cpp
        src/ThreadMonitor.cpp
        src/WindowManager.cpp)

include_directories("Dependencies/JSON")
include_directories("include")



# Add executable target with source files listed in SOURCE_FILES variable
add_executable(LighthouseKeeper ${SOURCE_FILES})
target_link_libraries(LighthouseKeeper tray)

if (UNIX)
    target_link_libraries(LighthouseKeeper stdc++fs pthread)
endif (UNIX)
if (WIN32)
    target_link_libraries(LighthouseKeeper Ws2_32 Mswsock Advapi32)
endif (WIN32)


add_custom_command(TARGET LighthouseKeeper PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/TemplateFiles $<TARGET_FILE_DIR:LighthouseKeeper>)



