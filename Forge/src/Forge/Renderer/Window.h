
#ifndef WINDOW_H
#define WINDOW_H

#include <GLFW/glfw3.h>

#include <cstdint>
#include <functional>
#include <string>

#include "Forge/Events/Event.h"

namespace Forge {

struct WindowAttributes {
    uint32_t width;
    uint32_t height;
    std::string name;

    WindowAttributes(const std::string& name = "Forge", uint32_t width = 1920, uint32_t height = 1080)
        : name(name)
        , width(width)
        , height(height) {}
};

class Window {
private:
    using EventCallbackFn = std::function<void(Event&)>;

public:
    Window(const WindowAttributes& attributes = WindowAttributes());
    ~Window();
    void* GetNativeWindow() const {
        return m_Window;
    };

    void Update();

    void SetEventCallback(const EventCallbackFn& callback) {
        m_WindowData.m_EventCallback = callback;
    }
    void EnableVSynk(bool enable);

private:
    struct WindowData {
        std::string Name;
        uint32_t Width;
        uint32_t Height;
        EventCallbackFn m_EventCallback;
    };

    void SetCallBackEvents();

private:
    GLFWwindow* m_Window;
    WindowAttributes m_WindowAtributes;
    WindowData m_WindowData;
};

} // namespace Forge
#endif
