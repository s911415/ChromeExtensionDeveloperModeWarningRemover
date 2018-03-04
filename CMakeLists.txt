# cmake_minimum_required(VERSION <specify CMake version here>)
project(ChromeExtensionDeveloperModeWarningRemover)

set(CMAKE_CXX_STANDARD 14)
set(BUILD_SHARED_LIBRARIES OFF)
set(CMAKE_EXE_LINKER_FLAGS "-static")
add_executable(ChromeExtensionDeveloperModeWarningRemover func.h func.cpp main.cpp)