//
// Created by toor on 2024-08-26.
//
#include "FileWatch.h"
#include "Forge/Core/Log/Log.h"
#include <iostream>
#include <cstring>
#include <filesystem>
#include <unistd.h>
#include <fcntl.h>

namespace Forge {
namespace FileWatch {
#if __unix__


Watch::Watch(const std::string& path, WatchCallback watchCallback, bool recursive) :
    m_watchCallback(std::move(watchCallback)), m_stopFlag(false), m_recursive(recursive)
{
    m_inotifyFd = inotify_init1(IN_NONBLOCK);
    if (m_inotifyFd == -1)
    {
        LOG_CRITICAL("Failed to initialize inotify: {}", std::string(strerror(errno)));
    }

    if (!addWatch(path))
    {
        close(m_inotifyFd);
        LOG_CRITICAL("Failed to add watch for path: {} ", path);
    }

    if (m_recursive)
    {
        recursiveAddWatch(path);
    }

    m_workingThread = std::thread(&Watch::run, this);
}

Watch::~Watch()
{
    m_stopFlag.store(true);
    if (m_workingThread.joinable())
    {
        m_workingThread.join();
    }
    close(m_inotifyFd);
}

bool Watch::addWatch(const std::string& path)
{
    int wd = inotify_add_watch(
        m_inotifyFd,
        path.c_str(),
        IN_MODIFY | IN_CREATE | IN_DELETE | IN_MOVED_FROM | IN_MOVED_TO | IN_ACCESS);
    if (wd == -1)
    {
        std::cerr << "Failed to add watch for " << path << ": " << strerror(errno) << std::endl;
        return false;
    }
    m_watchDescriptors[wd] = path;
    return true;
}

bool Watch::removeWatch(const std::string& path)
{
    for (const auto& pair : m_watchDescriptors)
    {
        if (pair.second == path)
        {
            if (inotify_rm_watch(m_inotifyFd, pair.first) == -1)
            {
                std::cerr << "Failed to remove watch for " << path << ": " << strerror(errno)
                          << std::endl;
                return false;
            }
            m_watchDescriptors.erase(pair.first);
            return true;
        }
    }
    return false;
}

void Watch::run()
{
    char buffer[BUFFER_SIZE];
    while (!m_stopFlag.load())
    {
        int length = read(m_inotifyFd, buffer, BUFFER_SIZE);
        if (length == -1)
        {
            if (errno == EAGAIN)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                continue;
            }
            std::cerr << "Error reading inotify events: " << strerror(errno) << std::endl;
            break;
        }

        int i = 0;
        while (i < length)
        {
            inotify_event* event = reinterpret_cast<inotify_event*>(&buffer[i]);
            WatchEvent watchEvent = maskToEvent(event->mask);

            m_watchCallback(watchEvent, std::string(event->name));

            if (m_recursive && (event->mask & IN_CREATE) && (event->mask & IN_ISDIR))
            {
                std::string path = m_watchDescriptors[event->wd];
                recursiveAddWatch(path);
            }
            i += sizeof(inotify_event) + event->len;
        }
    }
}

WatchEvent Watch::maskToEvent(uint32_t mask)
{
    if (mask & IN_CREATE)
        return WatchEvent::Created;
    if (mask & IN_DELETE)
        return WatchEvent::Removed;
    if (mask & IN_MODIFY)
        return WatchEvent::Modified;
    if (mask & IN_ACCESS)
        return WatchEvent::Accessed;
    if (mask & IN_MOVED_FROM)
        return WatchEvent::MovedFrom;
    if (mask & IN_MOVED_TO)
        return WatchEvent::MovedTo;
    return WatchEvent::Unknown;
}


void Watch::recursiveAddWatch(const std::string& path)
{
    for (const auto& entry : std::filesystem::recursive_directory_iterator(path))
    {
        if (entry.is_directory())
        {
            addWatch(entry.path().string());
        }
    }
}
}

}  // namespace Forge::FileWatch

#endif  // __unix__
