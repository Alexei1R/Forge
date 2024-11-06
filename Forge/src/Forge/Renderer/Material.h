//
// Created by toor on 2024-11-05.
//

#ifndef MATERIAL_H
#define MATERIAL_H


#include "Forge/Renderer/Handle.h"
#include "Forge/Renderer/Shader.h"
#include "Forge/Renderer/Texture.h"
#include "Forge/Core/Log/Log.h"
#include "glm/fwd.hpp"
#include <memory>
#include <optional>
#include <string>
#include <variant>
#include <unordered_map>
#include <mutex>

namespace Forge {

// Material parameter types
using MaterialParamValue =
    std::variant<float, glm::vec2, glm::vec3, glm::vec4, int, bool, std::shared_ptr<Texture>>;

struct Material
{
    std::string Name = "UnnamedMaterial";
    const Handle ShaderHandle;
    std::unordered_map<std::string, MaterialParamValue> aditionalParams;

    Material(const Handle& shaderHandle) : ShaderHandle(shaderHandle)
    {
        // Initialize default parameters
        aditionalParams["Color"] = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
        aditionalParams["Roughness"] = 0.5f;
        aditionalParams["Metalness"] = 0.0f;
    }

    ~Material() {}

    template <typename T>
    void SetParameter(const std::string& name, T value)
    {
        aditionalParams[name] = value;
    }

    template <typename T>
    std::optional<T> GetParameter(const std::string& name) const
    {
        auto it = aditionalParams.find(name);
        if (it != aditionalParams.end())
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
