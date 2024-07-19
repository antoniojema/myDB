# CMake minimum version check
cmake_minimum_required(VERSION 3.18)
#C++ standard required
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# (Windows) Use folders
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Project name
project("MyDatabase")
set(RootProjectPath "${PROJECT_SOURCE_DIR}")
message(STATUS "> Root Project Path: " ${RootProjectPath})

# Include source code
add_subdirectory(src)
