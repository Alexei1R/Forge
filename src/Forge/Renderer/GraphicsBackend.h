//
// Created by toor on 2024-07-30.
//

#ifndef GRAPHICS_BACKEND_H
#define GRAPHICS_BACKEND_H

#include <mutex>

namespace Forge {

enum class BackendTarget {
    None,
    Vulkan,
    OpenGL,
    Metal,
    DirectX,
};

class GraphicsBackend {
   public:
    GraphicsBackend() = default;

    static BackendTarget GetSelectedBackendTarget() {
        std::lock_guard<std::mutex> lock(m_Mutex);
        return m_BackendTarget;
    }

    static void SetBackendAPI(BackendTarget target) {
        std::lock_guard<std::mutex> lock(m_Mutex);
        m_BackendTarget = target;
    }

   private:
    static BackendTarget m_BackendTarget;
    static std::mutex m_Mutex;
};

}  // namespace Forge

#endif  // GRAPHICS_BACKEND_H
