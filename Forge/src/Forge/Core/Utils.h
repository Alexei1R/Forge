
#ifndef UTILS_H
#define UTILS_H

#include <iostream>

// Platform-specific debug break
#if defined(__linux__) && (defined(__GNUC__) || defined(__clang__))
#    define DEBUG_BREAK() std::abort()
#elif defined(_MSC_VER)
#    define DEBUG_BREAK() __debugbreak()
#else
#    define DEBUG_BREAK() std::abort()
#endif

// Assertion macro with message
#define F_ASSERT(value, message)                                                                                        \
    if (!(value)) {                                                                                                     \
        std::cerr << "[CRITICAL ERROR]\n FILE " << __FILE__ << "\n LINE :" << __LINE__ << "\t" << message << std::endl; \
        DEBUG_BREAK();                                                                                                  \
    }

#endif
