//
// Created by toor on 2024-07-21 .
//

#ifndef LOGGER_H
#define LOGGER_H
#include "spdlog/fmt/ostr.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"
#include <memory>

namespace Forge {

class Log {
   public:
    static void Init(std::string name);

    inline static std::shared_ptr<spdlog::logger> &GetLogger() { return s_Logger; }

   private:
    static std::shared_ptr<spdlog::logger> s_Logger;
};

}  // namespace Forge

#endif
