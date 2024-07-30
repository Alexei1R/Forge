//
// Created by toor on 2024-07-30 .
//

#include "Application.h"
#include <memory>
#include "Forge/Utils.h"

namespace Forge {

Application::Application() {
    Log::Init("Forge Logger");
    LOG_INFO("Init Logger")

    m_Window = Window::Create();
}

Application::~Application() {}

void Application::Run() {
    LOG_INFO("Before While")

    while (true) {
    }
}

}  // namespace Forge
