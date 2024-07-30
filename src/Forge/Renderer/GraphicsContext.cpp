//
// Created by toor on 2024-07-24 .
//

#include "GraphicsContext.h"
#include "Forge/Utils.h"
#include "Forge/Renderer/GraphicsBackend.h"

namespace Forge {

std::unique_ptr<GraphicsContext> GraphicsContext::Create() {
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
