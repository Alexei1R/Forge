
#ifndef MATERIAL_H
#define MATERIAL_H

#include "Forge/Config/Config.h"
#include "Forge/Config/Entry.h"
#include "Forge/Core/Handle.h"
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

enum class DrawPrimitives : uint32_t
{
    POINTS,
    LINES,
    TRIANGLES,
};

enum class CullFace : uint32_t
{
    NONE = 0,
    FRONT,
    BACK,
    FRONT_AND_BACK
};

enum class BlendingMode : uint32_t
{
    NONE = 0,
    ADD,
    SUBTRACT,
    REVERSE_SUBTRACT
};

using MaterialParamValue = std::variant<float, glm::vec2, glm::vec3, glm::vec4, int, bool>;

struct Material
{
    std::string Name = "UnnamedMaterial";

    Handle ShaderHandle;
    std::vector<Handle> TextureHandles;
    std::unordered_map<std::string, MaterialParamValue> aditionlParameters;


    glm::vec4 Color = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
    glm::vec3 EmissiveColor = glm::vec3(0.0f);

    float Specular = 0.5f;
    float Metallic = 0.0f;
    float Roughness = 0.5f;


    bool IsTransparent = false;
    bool DepthTestEnabled = true;
    bool CullFaceEnabled = true;
    DrawPrimitives PrimitiveType = DrawPrimitives::TRIANGLES;
    BlendingMode Blending = BlendingMode::NONE;
    CullFace CullFaceMode = CullFace::BACK;


    Material(const Handle& shaderHandle);


    ~Material() {}


    void SetTexture(uint32_t slot, const Handle& textureHandle);
    uint32_t GetTexureSlot(Handle handle);
    void ApplyRenderState() const;
    void Serialize();

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

    std::shared_ptr<Material> CreateMaterial(const std::string& name, const Handle& handle);
    std::shared_ptr<Material> GetMaterial(const std::string& name) const;
    void RemoveMaterial(const std::string& name);
    void ClearAll();

    void Serialize();

    void Deserialize();

private:
    mutable std::mutex mutex_;  // Mutex for thread safety
    std::unordered_map<std::string, std::shared_ptr<Material>> materialComponents;
};

}  // namespace Forge


#endif
