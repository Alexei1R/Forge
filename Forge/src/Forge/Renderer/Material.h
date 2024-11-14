//
// Created by toor on 2024-11-05.
//

#ifndef MATERIAL_H
#define MATERIAL_H

#include "Forge/Renderer/Handle.h"
#include "Forge/Renderer/RenderCommand.h"
#include "Forge/Core/Log/Log.h"
#include "glm/fwd.hpp"
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <variant>
#include <unordered_map>
#include <mutex>
#include <vector>

namespace Forge {

using MaterialParamValue = std::variant<float, glm::vec2, glm::vec3, glm::vec4, int, bool>;

struct Material
{
    std::string Name = "UnnamedMaterial";

    const Handle ShaderHandle;
    std::vector<Handle> TextureHandles;
    std::unordered_map<std::string, MaterialParamValue> aditionlParameters;


    glm::vec4 Color = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
    glm::vec3 EmissiveColor = glm::vec3(0.0f);

    float Metallic = 0.0f;
    float Roughness = 0.5f;
    float Specular = 0.5f;
    float Opacity = 1.0f;
    float RefractionIndex = 1.0f;
    float Anisotropy = 0.0f;
    float SubsurfaceScattering = 0.0f;

    Material(const Handle& shaderHandle) : ShaderHandle(shaderHandle)
    {
        TextureHandles.resize(RenderCommand::GetMaxTextureSlots());
    }


    void SetTexture(uint32_t slot, const Handle& textureHandle)
    {
        if (textureHandle == Handle(0))
        {
            LOG_CRITICAL("Invalid textureHandle ")
            return;
        }

        if (slot < TextureHandles.size())
        {
            TextureHandles[slot] = textureHandle;
        }
        else
        {
            LOG_ERROR("Texture slot {} exceeds maximum of texture slots", slot);
        }
    }

    uint32_t GetTexureSlot(Handle handle)
    {
        for (uint32_t i = 0; i < TextureHandles.size(); ++i)
        {
            if (TextureHandles[i] == handle)
            {
                return i;
            }
        }
        LOG_WARN("Handle not found in TextureHandles.");
        return UINT32_MAX;  // Cod de eroare dacă nu este găsit
    }


    ~Material() {}

    template <typename T>
    void SetParameter(const std::string& name, T value)
    {
        aditionlParameters[name] = value;
    }

    template <typename T>
    std::optional<T> GetParameter(const std::string& name) const
    {
        auto it = aditionlParameters.find(name);
        if (it != aditionlParameters.end())
        {
            const MaterialParamValue& value = it->second;
            if (std::holds_alternative<T>(value))
            {
                return std::get<T>(value);
            }
        }
        return std::nullopt;
    }
};

class MaterialManager
{
public:
    MaterialManager() = default;
    ~MaterialManager()
    {
        ClearAll();
    }

    std::shared_ptr<Material> CreateMaterial(const std::string& name, const Handle& handle)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (materialComponents.find(name) != materialComponents.end())
        {
            return materialComponents[name];
        }

        auto material = std::make_shared<Material>(handle);
        material->Name = name;
        materialComponents[name] = material;
        LOG_INFO("Material '{}' created and added to MaterialManager.", name);
        return material;
    }

    std::shared_ptr<Material> GetMaterial(const std::string& name) const
    {
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = materialComponents.find(name);
        if (it != materialComponents.end())
        {
            return it->second;
        }
        LOG_ERROR("Material '{}' not found in MaterialManager.", name);
        return nullptr;
    }

    void RemoveMaterial(const std::string& name)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (!materialComponents.erase(name))
        {
            LOG_WARN("Material '{}' not found in MaterialManager.", name);
        }
    }

    void ClearAll()
    {
        std::lock_guard<std::mutex> lock(mutex_);
        materialComponents.clear();
    }

private:
    mutable std::mutex mutex_;  // Mutex for thread safety
    std::unordered_map<std::string, std::shared_ptr<Material>> materialComponents;
};

}  // namespace Forge


#endif
