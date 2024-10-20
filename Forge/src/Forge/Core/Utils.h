//
// Created by toor on 2024-08-26.
//

#ifndef UTILS_H
#define UTILS_H

#include <iostream>

// clang-format off


// Platform-specific debug break
#if defined(__linux__) && (defined(__GNUC__) || defined(__clang__))
#define DEBUG_BREAK() std::abort()
#elif defined(_MSC_VER)
#define DEBUG_BREAK() __debugbreak()
#else
#define DEBUG_BREAK() std::abort()
#endif

// Assertion macro with message
#define F_ASSERT(value, message) if (!(value)) { std::cerr << message << std::endl; DEBUG_BREAK();}

// clang-format on


#endif
