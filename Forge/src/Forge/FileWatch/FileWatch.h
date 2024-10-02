//
// Created by toor on 2024-08-26.
//

#ifndef FILEWATCH_H
#define FILEWATCH_H

#include <atomic>
#include <functional>
#include <string>
#include <thread>
#include <unordered_map>

#if __unix__
#    include <sys/inotify.h>
#    include <linux/limits.h>
#endif  // __unix__

namespace Forge { namespace FileWatch {

enum class WatchEvent
{
    Unknown,
    Created,
    Removed,
    Modified,
    Accessed,
    MovedFrom,
    MovedTo
};

class Watch
{
public:
    using WatchCallback = std::function<void(WatchEvent, const std::string&)>;

    Watch(const std::string& path, WatchCallback watchCallback, bool recursive = false);
    ~Watch();

    bool addWatch(const std::string& path);
    bool removeWatch(const std::string& path);

private:
    void run();
    WatchEvent maskToEvent(uint32_t mask);
    void recursiveAddWatch(const std::string& path);

    WatchCallback m_watchCallback;
    std::atomic<bool> m_stopFlag;
    std::thread m_workingThread;
    bool m_recursive;
#if __unix__
    int m_inotifyFd;
    std::unordered_map<int, std::string> m_watchDescriptors;
    static constexpr size_t BUFFER_SIZE = 4096;
#endif  // __unix__
};

}}  // namespace Forge::FileWatch
#endif  // FILEWATCH_H
