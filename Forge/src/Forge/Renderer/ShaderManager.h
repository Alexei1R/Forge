
#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include "Forge/Core/Handle.h"
#include "Shader.h"
#include <unordered_map>
#include <memory>
#include <mutex>

namespace Forge {

class ShaderManager
{
public:
    static ShaderManager& GetInstance();

    Handle LoadShader(const ShaderLayout& layout);
    Shader* GetShader(const Handle& handle) const noexcept;
    void RemoveShader(const Handle& handle) noexcept;
    void ClearCache() noexcept;

    void Shutdown() noexcept;

private:
    ShaderManager() = default;
    ~ShaderManager() = default;

    ShaderManager(const ShaderManager&) = delete;
    ShaderManager& operator=(const ShaderManager&) = delete;

    Handle GenerateHandle() noexcept;

    mutable std::mutex m_ShaderMutex;
    std::unordered_map<Handle, std::unique_ptr<Shader>> m_Shaders;
    uint32_t m_NextHandleValue = 1;
};

}  // namespace Forge

#endif  // SHADERMANAGER_H
