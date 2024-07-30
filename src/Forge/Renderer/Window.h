//
// Created by toor on 2024-07-21.
//

#ifndef WINDOW_H
#define WINDOW_H

#include <string>
#include <cstdint>
#include <memory>

namespace Forge {

struct WindowAttributes {
    uint32_t width;
    uint32_t height;
    std::string name;

    WindowAttributes(const std::string& name = "Forge", uint32_t width = 1920, uint32_t height = 1080)
        : name(name), width(width), height(height) {}
};

class Window {
   public:
    static std::shared_ptr<Window> Create(const WindowAttributes& attributes = WindowAttributes());
};

}  // namespace Forge

#endif  // WINDOW_H
