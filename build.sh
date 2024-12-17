#!/bin/bash

build_and_run() {
    if [ ! -d "build" ]; then
        mkdir build
    fi
    cd build || exit

    if cmake ..; then
        if make -j"$(nproc)"; then
            cd sandbox || exit
            if [ -f "./sandbox" ]; then
                case "$1" in
                    -pcpu)
                        notify-send -a "Forge" "Starting CPU profiling..."
                        perf record --call-graph dwarf ./sandbox
                        notify-send -a "Forge" "CPU profiling completed."
                        if [ -f "perf.data" ]; then
                            notify-send -a "Forge" "Launching Hotspot with perf.data..."
                            hotspot perf.data
                        else
                            notify-send -a "Forge" "Perf data not found. Run with -pcpu first."
                        fi
                        ;;
                    -pgpu)
                        notify-send -a "Forge" "Starting GPU profiling..."
                        echo "There is no gpu profiling"
                        ;;
                    *)
                        ./sandbox
                        ;;
                esac
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

case "$1" in
    -pcpu)
        build_and_run -pcpu
        ;;
    -pgpu)
        build_and_run -pgpu
        ;;
    *)
        build_and_run
        ;;
esac
