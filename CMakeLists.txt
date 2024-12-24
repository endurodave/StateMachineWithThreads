# Example CMake command line to create project build files:
#
# *** Windows ***
# cmake -G "Visual Studio 17 2022" -A Win32 -B Build -S .
#
# *** Linux ***
# cmake -G "Unix Makefiles" -B Build -S .

# Specify the minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name and language (C or C++)
project(StateMachineWithThreads VERSION 1.0 LANGUAGES CXX)

# Set C++ standard
#set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Collect all .cpp and *.h source files in the current directory
file(GLOB SOURCES "${CMAKE_SOURCE_DIR}/*.cpp" "${CMAKE_SOURCE_DIR}/*.h")

# Add subdirectories to include path
include_directories( 
    ${CMAKE_SOURCE_DIR}/AsyncCallback
    ${CMAKE_SOURCE_DIR}/PortWin
    ${CMAKE_SOURCE_DIR}/SelfTest
    ${CMAKE_SOURCE_DIR}/StateMachine
    ${CMAKE_SOURCE_DIR}/Util
)

# Add an executable target
add_executable(StateMachineWithThreadsApp ${SOURCES})

# Link against Windows multimedia library (winmm.lib)
if (WIN32)
    target_link_libraries(StateMachineWithThreadsApp PRIVATE winmm)
endif()

# Add subdirectories to build
add_subdirectory(AsyncCallback)
add_subdirectory(PortWin)
add_subdirectory(SelfTest)
add_subdirectory(StateMachine)
add_subdirectory(Util)

target_link_libraries(StateMachineWithThreadsApp PRIVATE 
    AsyncCallbackLib
    PortWinLib
    SelfTestLib
    StateMachineLib
    UtilLib
)

