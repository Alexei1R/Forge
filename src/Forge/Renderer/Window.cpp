//
// Created by toor on 2024-07-21.
//

#include "Window.h"
#include "Forge/Utils.h"
#include "Forge/Renderer/GraphicsBackend.h"

namespace Forge {

std::shared_ptr<Window> Window::Create(const WindowAttributes& attributes) {
    auto api = GraphicsBackend::GetSelectedBackendTarget();
    switch (api) {
        case BackendTarget::OpenGL:
            LOG_INFO("Creating OPENGL Window")
            return nullptr;
        case BackendTarget::Vulkan:
            return nullptr;
        case BackendTarget::Metal:
            return nullptr;
        case BackendTarget::DirectX:
            return nullptr;
        case BackendTarget::None:
            LOG_CRITICAL("No API specified")
            F_ASSERT(true, "There is no default api , Please specify the BackendTarget")

            return nullptr;
        default:
            F_ASSERT(true, "There is no default api , Please specify the BackendTarget")
            return nullptr;
    }
}

}  // namespace Forge
