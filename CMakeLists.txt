cmake_minimum_required(VERSION 3.15)

project(Forge
    VERSION 0.0.1
    LANGUAGES CXX
)

# Set C++ Standard and other options
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set default build type if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE  Release)  # Choose Release, Debug, etc.
endif()

find_path(Forge_LIBS
  NAMES "Forge"
  PATHS "${CMAKE_SOURCE_DIR}/Forge/src/"
  NO_DEFAULT_PATH
)

# Check if the directory was found
if (Forge_LIBS)
    message(STATUS "Forge  directory found at: ${Forge_LIBS}")
else()
    message(FATAL_ERROR "Forge  directory not found!")
endif()



# Include subdirectories
add_subdirectory(Forge)
add_subdirectory(Editor)


