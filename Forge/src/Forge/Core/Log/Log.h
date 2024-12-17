#ifndef LOG_H
#define LOG_H

#include <cstdlib>
#include <iostream>
#include <memory>

#include "Forge/Core/Utils.h"
#include "LogFormaters.h"
#include "spdlog/fmt/ostr.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"

namespace Forge {

class Log {
public:
    static void Init(std::string name);

    inline static std::shared_ptr<spdlog::logger>& GetLogger() {
        return s_Logger;
    }

private:
    static std::shared_ptr<spdlog::logger> s_Logger;
};

} // namespace Forge

// clang-format off
#ifndef NULL
#define LOG_TRACE(...)
#define LOG_INFO(...)
#define LOG_WARN(...)
#define LOG_ERROR(...)
#define LOG_CRITICAL(...)
#else
// Core log macros
#define LOG_TRACE(...) F_ASSERT(::Forge::Log::GetLogger(), " Logger is not Init")::Forge::Log::GetLogger()->trace(__VA_ARGS__);
#define LOG_INFO(...) F_ASSERT(::Forge::Log::GetLogger(), " Logger is not Init")::Forge::Log::GetLogger()->info(__VA_ARGS__);
#define LOG_WARN(...) F_ASSERT(::Forge::Log::GetLogger(), " Logger is not Init")::Forge::Log::GetLogger()->warn(__VA_ARGS__);
#define LOG_ERROR(...) F_ASSERT(::Forge::Log::GetLogger(), " Logger is not Init")::Forge::Log::GetLogger()->error(__VA_ARGS__);
#define LOG_CRITICAL(...) F_ASSERT(::Forge::Log::GetLogger(), " Logger is not Init")::Forge::Log::GetLogger()->critical("{} :: {}",__VA_ARGS__ , __FILE__);

#define LOG_INITIALIZED
#endif
// clang-format on

#endif
