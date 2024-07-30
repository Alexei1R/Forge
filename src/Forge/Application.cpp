//
// Created by toor on 2024-07-30 .
//

#include "Application.h"
#include <memory>
#include "Forge/Renderer/GraphicsBackend.h"
#include "Forge/Utils.h"

namespace Forge {

Application::Application() {
    Log::Init("Forge Logger");
    LOG_INFO("Init Logger")

    GraphicsBackend::SetBackendAPI(BackendTarget::OpenGL);
    m_Window = std::make_shared<Window>();
    m_Window->SetEventCallback(std::bind(&Application::HandleEvent, this, std::placeholders::_1));
}

Application::~Application() {}

void Application::Run() {
    LOG_INFO("Before While")

    while (true) {
    }
}

void Application::HandleEvent(const Event &event) {
    if (event.GetType() == EventType::Window) {
        WindowEvent windowEvent = static_cast<const WindowEvent &>(event);
        if (windowEvent.GetAction() == Action::Close) {
            LOG_INFO("Window Closed")
        }
    }

    if (event.GetType() == EventType::Key) {
        KeyEvent windowEvent = static_cast<const KeyEvent &>(event);
        if (windowEvent.GetAction() == Action::KeyRelease) {
            LOG_INFO("KeyRelease {0}", windowEvent.GetKey())
        }
    }
};

}  // namespace Forge
