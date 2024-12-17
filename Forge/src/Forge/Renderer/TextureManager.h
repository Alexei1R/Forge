
#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <filesystem>
#include <mutex>
#include <string>

#include "Forge/Core/Handle.h"
#include "Forge/Renderer/Texture.h"
namespace Forge {

class TextureManager {
public:
    static TextureManager& GetInstance();

    Handle LoadTexture(const std::filesystem::path path);
    Handle DataTexture(int width, int height, GLenum format, const void* data = nullptr);
    Texture* GetTexture(const Handle& handle) const noexcept;
    void RemoveTexture(const Handle& handle) noexcept;
    void ClearCache() noexcept;
    void Shutdown() noexcept;

private:
    TextureManager() = default;
    ~TextureManager() = default;

    TextureManager(const TextureManager&) = delete;
    TextureManager& operator=(const TextureManager&) = delete;

    Handle GenerateHandle() noexcept;

    mutable std::mutex m_TextureMutex;
    std::unordered_map<Handle, std::unique_ptr<Texture>> m_Textures;
    uint32_t m_NextHandleValue = 1;
};
} // namespace Forge

#endif
