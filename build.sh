#!/bin/bash

build_and_run() {
    if [ ! -d "build" ]; then
        mkdir build
    fi
    cd build || exit

    if cmake ..; then
        if make -j"$(nproc)"; then
            cd sandbox || exit
            if [ -f "./executable" ]; then
                ./executable
            else
                notify-send "ATOM" "Build successful, but executable not found."
            fi
        else
            notify-send "ATOM" "Make failed."
            exit 1
        fi
    else
        notify-send "ATOM" "CMake failed."
        exit 1
    fi
}

notify-send "ATOM" "Begin build."
build_and_run

