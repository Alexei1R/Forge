

#include "Window.h"

#include <GLFW/glfw3.h>

#include <string>
#include <vector>

#include "Forge/Core/Log/Log.h"
#include "Forge/Core/Utils.h"
#include "Forge/Events/Event.h"
#include "Forge/Events/ImplEvent.h"

namespace Forge {

Window::Window(const WindowAttributes& attributes)
    : m_WindowAtributes(attributes) {
    m_WindowData.Name = attributes.name;
    m_WindowData.Width = attributes.width;
    m_WindowData.Height = attributes.height;

    F_ASSERT(glfwInit(), "ERROR Init GLFW")

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    m_Window = glfwCreateWindow(m_WindowData.Width, m_WindowData.Height, m_WindowData.Name.c_str(), nullptr, nullptr);
    F_ASSERT(m_Window, "ERROR to create window")
    LOG_INFO("Creating Window ::  {0}, {1}:{2}", m_WindowData.Name, m_WindowData.Width, m_WindowData.Height);
    glfwMakeContextCurrent(m_Window);

    glfwSetWindowUserPointer(m_Window, &m_WindowData);
    SetCallBackEvents();
}

Window::~Window() {
    glfwDestroyWindow(m_Window);
    glfwTerminate();
}

void Window::Update() {
    glfwPollEvents();
}

void Window::EnableVSynk(bool enable) {
    if (enable)
        glfwSwapInterval(1);
    else
        glfwSwapInterval(0);
}

void Window::SetCallBackEvents() {
    // Set GLFW callbacks

    //
    // Window Events
    //
    glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        data.Width = width;
        data.Height = height;

        WindowEvent event(width, height, Action::Resize);
        data.m_EventCallback(event);
    });
    glfwSetWindowPosCallback(m_Window, [](GLFWwindow* window, int xPos, int yPos) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        WindowEvent event(xPos, yPos, Action::Move);
        data.m_EventCallback(event);
    });

    glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        WindowEvent event(0, 0, Action::Close);
        data.m_EventCallback(event);
    });

    glfwSetWindowFocusCallback(m_Window, [](GLFWwindow* window, int focused) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        Action action = focused ? Action::Focus : Action::LoseFocus;
        WindowEvent event(0, 0, action);
        data.m_EventCallback(event);
    });

    glfwSetWindowIconifyCallback(m_Window, [](GLFWwindow* window, int iconified) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        Action action = iconified ? Action::Iconify : Action::Restore;
        WindowEvent event(0, 0, action);
        data.m_EventCallback(event);
    });

    glfwSetWindowMaximizeCallback(m_Window, [](GLFWwindow* window, int maximized) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        Action action = maximized ? Action::Maximize : Action::Restore;
        WindowEvent event(0, 0, action);
        data.m_EventCallback(event);
    });

    glfwSetFramebufferSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        WindowEvent event(width, height, Action::FramebufferResize);
        data.m_EventCallback(event);
    });

    //
    // Key Events
    //

    glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        switch (action) {
        case GLFW_PRESS: {
            KeyEvent event(key, Action::KeyPress);
            data.m_EventCallback(event);

            break;
        }
        case GLFW_RELEASE: {
            KeyEvent event(key, Action::KeyRelease);
            data.m_EventCallback(event);
            break;
        }
        case GLFW_REPEAT: {
            KeyEvent event(key, Action::KeyRepeat);
            data.m_EventCallback(event);
            break;
        }
        }
    });

    glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        KeyEvent event(keycode, Action::RegisterKeyChar);
        data.m_EventCallback(event);
    });

    glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        switch (action) {
        case GLFW_PRESS: {
            KeyEvent event(button, Action::KeyPress);
            data.m_EventCallback(event);
            break;
        }
        case GLFW_RELEASE: {
            KeyEvent event(button, Action::KeyRelease);
            data.m_EventCallback(event);
            break;
        }
        }
    });

    glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        MouseEvent event(xOffset, yOffset, Action::MouseScroll);
        data.m_EventCallback(event);
    });

    glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        MouseEvent event(xPos, yPos, Action::MouseMove);
        data.m_EventCallback(event);
    });

    glfwSetDropCallback(m_Window, [](GLFWwindow* window, int count, const char* paths[]) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        std::vector<std::string> droppedFiles;
        droppedFiles.reserve(count);

        for (int i = 0; i < count; ++i) {
            droppedFiles.emplace_back(paths[i]);
        }
        DropEvent event(droppedFiles, Action::Drop);
        data.m_EventCallback(event);
    });
}

} // namespace Forge
