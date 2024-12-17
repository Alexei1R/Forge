

#include "ShaderManager.h"

#include "Forge/Core/Log/Log.h"

namespace Forge {

ShaderManager& ShaderManager::GetInstance() {
    static ShaderManager instance;
    return instance;
}

void ShaderManager::Shutdown() noexcept {
    std::lock_guard<std::mutex> lock(m_ShaderMutex);
    m_Shaders.clear();
}

Handle ShaderManager::LoadShader(const ShaderLayout& layout) {
    std::lock_guard<std::mutex> lock(m_ShaderMutex);
    Handle newHandle = GenerateHandle();

    auto shader = std::make_unique<Shader>(layout);
    try {
        shader->Build();
        m_Shaders.emplace(newHandle, std::move(shader));
        return newHandle;
    } catch (const std::runtime_error& e) {
        LOG_CRITICAL("Failed to compile shader with handle {}: {}", newHandle.GetValue(), e.what());
        return Handle(0); // Returning a null/invalid handle on failure
    }
}

Shader* ShaderManager::GetShader(const Handle& handle) const noexcept {
    std::lock_guard<std::mutex> lock(m_ShaderMutex);
    auto it = m_Shaders.find(handle);
    return (it != m_Shaders.end()) ? it->second.get() : nullptr;
}

void ShaderManager::RemoveShader(const Handle& handle) noexcept {
    std::lock_guard<std::mutex> lock(m_ShaderMutex);
    m_Shaders.erase(handle);
}

void ShaderManager::ClearCache() noexcept {
    std::lock_guard<std::mutex> lock(m_ShaderMutex);
    m_Shaders.clear();
}

Handle ShaderManager::GenerateHandle() noexcept {
    return Handle(m_NextHandleValue++);
}

} // namespace Forge
