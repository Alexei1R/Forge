#include "Log.h"
namespace Forge {

std::shared_ptr<spdlog::logger> Log::s_Logger = nullptr;

void Log::Init(std::string name) {
    spdlog::set_pattern("%^[%T] %n: %v%$");
    s_Logger = spdlog::stdout_color_mt(name);
    s_Logger->set_level(spdlog::level::trace);
}

} // namespace Forge
