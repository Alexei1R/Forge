cmake_minimum_required(VERSION 3.15)
project(glad)

# Add the source files
set(SOURCES
        "include/glad/glad.h"
        "include/glad/khrplatform.h"

        "src/glad.c"
)


# Create a static library
add_library(glad STATIC ${SOURCES})

# Include directories
target_include_directories(glad PUBLIC "include")
