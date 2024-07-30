//
// Created by toor on 2024-07-30 .
//

#ifndef UTILS_H
#define UTILS_H

#include <cstdlib>
#include <iostream>

#include "Forge/Logger/Logger.h"

// Assertion macro with message
// clang-format off
#define F_ASSERT(value, message) if (!(value)) { std::cerr << message << std::endl; DEBUG_BREAK();}
// clang-format on

#ifdef NDEBUG
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
#define LOG_CRITICAL(...) \
    F_ASSERT(::Forge::Log::GetLogger(), " Logger is not Init")::Forge::Log::GetLogger()->critical(__VA_ARGS__);
#endif

// Platform-specific debug break
#if defined(__linux__) && (defined(__GNUC__) || defined(__clang__))
#define DEBUG_BREAK() std::abort()
#elif defined(_MSC_VER)
#define DEBUG_BREAK() __debugbreak()
#else
#define DEBUG_BREAK() std::abort()
#endif

#endif  // UTILS_H
