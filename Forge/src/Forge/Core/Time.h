
#ifndef TIME_H
#define TIME_H

#include <chrono>
namespace Forge {


using DeltaTime = float;


class Timer
{
private:
    std::chrono::steady_clock::time_point startTime;
    std::chrono::steady_clock::time_point lastTime;

public:
    Timer()
    {
        reset();
    }

    void reset()
    {
        startTime = std::chrono::steady_clock::now();
        lastTime = startTime;
    }

    double getDeltaTime()
    {
        auto currentTime = std::chrono::steady_clock::now();
        auto deltaTime = std::chrono::duration<double>(currentTime - lastTime).count();
        lastTime = currentTime;
        return deltaTime;
    }

    double getElapsedTime()
    {
        auto currentTime = std::chrono::steady_clock::now();
        return std::chrono::duration<double>(currentTime - startTime).count();
    }
};
}  // namespace Forge


#endif
