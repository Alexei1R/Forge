

#include "TextureManager.h"

#include "Forge/Core/Log/Log.h"

namespace Forge {

TextureManager& TextureManager::GetInstance() {
    static TextureManager instance;
    return instance;
}

void TextureManager::Shutdown() noexcept {
    std::lock_guard<std::mutex> lock(m_TextureMutex);
    m_Textures.clear();
}

Handle TextureManager::LoadTexture(const std::filesystem::path path) {
    std::lock_guard<std::mutex> lock(m_TextureMutex);
    Handle newHandle = GenerateHandle();

    try {
        auto texture = std::make_unique<Texture>(path);
        m_Textures.emplace(newHandle, std::move(texture));
        return newHandle;
    } catch (const std::runtime_error& e) {
        LOG_CRITICAL("Failed to load with handle {}: {}", newHandle.GetValue(), e.what());
        return Handle(0);
    }
}

Handle TextureManager::DataTexture(int width, int height, GLenum format, const void* data) {
    std::lock_guard<std::mutex> lock(m_TextureMutex);
    Handle newHandle = GenerateHandle();

    try {
        auto texture = std::make_unique<Texture>(width, height, format, data);
        m_Textures.emplace(newHandle, std::move(texture));
        return newHandle;
    } catch (const std::runtime_error& e) {
        LOG_CRITICAL("Failed to load with handle {}: {}", newHandle.GetValue(), e.what());
        return Handle(0);
    }
}

Texture* TextureManager::GetTexture(const Handle& handle) const noexcept {
    std::lock_guard<std::mutex> lock(m_TextureMutex);
    auto it = m_Textures.find(handle);
    return (it != m_Textures.end()) ? it->second.get() : nullptr;
}

void TextureManager::RemoveTexture(const Handle& handle) noexcept {
    std::lock_guard<std::mutex> lock(m_TextureMutex);
    m_Textures.erase(handle);
}

void TextureManager::ClearCache() noexcept {
    std::lock_guard<std::mutex> lock(m_TextureMutex);
    m_Textures.clear();
}

Handle TextureManager::GenerateHandle() noexcept {
    return Handle(m_NextHandleValue++);
}

} // namespace Forge
