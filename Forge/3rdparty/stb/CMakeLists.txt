
cmake_minimum_required(VERSION 3.15)
project(stb)

# Add the source files
set(SOURCES
        "include/stb/stb_image.h"

        "src/stb_image.cpp"
)


# Create a static library
add_library(stb STATIC ${SOURCES})

# Include directories
target_include_directories(stb PUBLIC "include")
