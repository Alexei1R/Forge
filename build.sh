#!/bin/bash

build_and_run() {
    if [ ! -a "build" ]; then
        mkdir build
    fi
    cd build || exit

    if cmake ..; then
        if make -j"$(nproc)"; then
            cd Editor || exit
            if [ -f "./ForgeEditor" ]; then
                ./ForgeEditor
            else
                notify-send -a "Forge" "Build successful, but executable not found."
            fi
        else
            notify-send -a "Forge" "Make failed."
            exit 1
        fi
    else
        notify-send -a "Forge" "CMake failed."
        exit 1
    fi
}

notify-send -a "Forge" "Begin build."
build_and_run



