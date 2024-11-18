//
// Created by toor on 2024-11-05.
//

#ifndef MATERIAL_H
#define MATERIAL_H

#include "Forge/Config/Config.h"
#include "Forge/Config/Entry.h"
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

    Handle ShaderHandle;
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


    /*bool IsTransparent = false;*/
    /*BlendingMode BlendingMode = BlendingMode::NONE;*/
    /*bool DepthTestEnabled = true;*/
    /*CullFace CullFaceMode = CullFace::BACK;*/
    /*bool CullFaceEnabled = true;*/


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

    void Serialize()
    {
        Config::Config config;

        // Salvăm proprietățile de bază ale materialului
        config.AddEntry("Color", Color);
        config.AddEntry("EmissiveColor", EmissiveColor);
        config.AddEntry("Metallic", Metallic);
        config.AddEntry("Roughness", Roughness);
        config.AddEntry("Specular", Specular);
        config.AddEntry("Opacity", Opacity);
        config.AddEntry("RefractionIndex", RefractionIndex);
        config.AddEntry("Anisotropy", Anisotropy);
        config.AddEntry("SubsurfaceScattering", SubsurfaceScattering);
        config.AddEntry("ShaderHandle", (int)ShaderHandle.GetValue());

        // Salvăm identificatorii texturilor
        for (size_t i = 0; i < TextureHandles.size(); ++i)
        {
            if (TextureHandles[i] != Handle(0))
            {
                config.AddEntry<int>("Texture" + std::to_string(i), TextureHandles[i].GetValue());
            }
        }

        // Salvăm parametrii suplimentari
        for (const auto& [key, value] : aditionlParameters)
        {
            if (std::holds_alternative<float>(value))
                config.AddEntry(key, std::get<float>(value));
            else if (std::holds_alternative<glm::vec2>(value))
                config.AddEntry(key, std::get<glm::vec2>(value));
            else if (std::holds_alternative<glm::vec3>(value))
                config.AddEntry(key, std::get<glm::vec3>(value));
            else if (std::holds_alternative<glm::vec4>(value))
                config.AddEntry(key, std::get<glm::vec4>(value));
            else if (std::holds_alternative<int>(value))
                config.AddEntry(key, std::get<int>(value));
            else if (std::holds_alternative<bool>(value))
                config.AddEntry(key, std::get<bool>(value));
        }

        // Salvăm fișierul în folderul "Assets/Material"
        std::filesystem::path current_path = std::filesystem::current_path();
        std::filesystem::path file_path = current_path / "Assets/Material" / (Name + ".mat");

        if (config.SaveToFile(file_path.string()))
        {
            LOG_INFO("Material '{}' saved successfully to '{}'.", Name, file_path.string());
        }
        else
        {
            LOG_ERROR("Failed to save material '{}' to '{}'.", Name, file_path.string());
        }
    }


    /*void ApplyRenderState() const*/
    /*{*/
    /*    if (IsTransparent)*/
    /*    {*/
    /*        glEnable(GL_BLEND);*/
    /*        switch (BlendingMode)*/
    /*        {*/
    /*            case BlendingMode::ADD: glBlendEquation(GL_FUNC_ADD); break;*/
    /*            case BlendingMode::SUBTRACT: glBlendEquation(GL_FUNC_SUBTRACT); break;*/
    /*            case BlendingMode::REVERSE_SUBTRACT:*/
    /*                glBlendEquation(GL_FUNC_REVERSE_SUBTRACT);*/
    /*                break;*/
    /*            case BlendingMode::NONE:*/
    /*            default: glDisable(GL_BLEND); break;*/
    /*        }*/
    /*    }*/
    /*    else*/
    /*    {*/
    /*        glDisable(GL_BLEND);*/
    /*    }*/
    /**/
    /*    if (DepthTestEnabled)*/
    /*    {*/
    /*        glEnable(GL_DEPTH_TEST);*/
    /*    }*/
    /*    else*/
    /*    {*/
    /*        glDisable(GL_DEPTH_TEST);*/
    /*    }*/
    /**/
    /*    if (CullFaceEnabled)*/
    /*    {*/
    /*        glEnable(GL_CULL_FACE);*/
    /*        switch (CullFaceMode)*/
    /*        {*/
    /*            case CullFace::FRONT: glCullFace(GL_FRONT); break;*/
    /*            case CullFace::BACK: glCullFace(GL_BACK); break;*/
    /*            case CullFace::FRONT_AND_BACK: glCullFace(GL_FRONT_AND_BACK); break;*/
    /*            case CullFace::NONE:*/
    /*            default: glDisable(GL_CULL_FACE); break;*/
    /*        }*/
    /*    }*/
    /*    else*/
    /*    {*/
    /*        glDisable(GL_CULL_FACE);*/
    /*    }*/
    /*}*/
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

    void Serialize()
    {
        for (auto& material : materialComponents)
        {
            material.second->Serialize();
        }
    }


    void Deserialize()
    {
        std::lock_guard<std::mutex> lock(mutex_);

        std::filesystem::path current_path = std::filesystem::current_path();
        std::filesystem::path material_folder = current_path / "Assets/Material";

        if (!std::filesystem::exists(material_folder))
        {
            LOG_ERROR("Material folder does not exist: {}", material_folder.string());
            return;
        }

        // Iterăm prin toate fișierele din folder
        for (const auto& entry : std::filesystem::directory_iterator(material_folder))
        {
            if (entry.is_regular_file() && entry.path().extension() == ".mat")
            {
                Config::Config config;
                if (!config.LoadFromFile(entry.path().string()))
                {
                    LOG_ERROR("Failed to load material file: {}", entry.path().string());
                    continue;
                }

                auto material = std::make_shared<Material>(Handle(0));
                material->Name = entry.path().stem().string();
                LOG_ERROR("Loading Material {}", entry.path().stem().string())

                try
                {
                    // Reconstruim proprietățile de bază
                    material->Color = config.GetEntry<glm::vec4>("Color")->GetData();
                    material->EmissiveColor =
                        config.GetEntry<glm::vec3>("EmissiveColor")->GetData();
                    material->Metallic = config.GetEntry<float>("Metallic")->GetData();
                    material->Roughness = config.GetEntry<float>("Roughness")->GetData();
                    material->Specular = config.GetEntry<float>("Specular")->GetData();
                    material->Opacity = config.GetEntry<float>("Opacity")->GetData();
                    material->RefractionIndex =
                        config.GetEntry<float>("RefractionIndex")->GetData();
                    material->Anisotropy = config.GetEntry<float>("Anisotropy")->GetData();
                    material->SubsurfaceScattering =
                        config.GetEntry<float>("SubsurfaceScattering")->GetData();
                    material->ShaderHandle =
                        Handle(config.GetEntry<int>("ShaderHandle")->GetData());

                    // Reconstruim identificatorii texturilor
                    for (size_t i = 0; i < material->TextureHandles.size(); ++i)
                    {
                        auto textureKey = "Texture" + std::to_string(i);
                        if (auto entry = config.GetEntry<int>(textureKey))
                        {
                            Handle textureHandle = Handle(entry->GetData());
                            if (textureHandle != Handle(0))
                            {
                                material->TextureHandles[i] = textureHandle;
                            }
                        }
                    }

                    // Reconstruim parametrii suplimentari
                    for (const auto& [key, value] : config.entries())
                    {
                        if (key == "Color" || key == "EmissiveColor" || key == "Metallic" ||
                            key == "Roughness" || key == "Specular" || key == "Opacity" ||
                            key == "RefractionIndex" || key == "Anisotropy" ||
                            key == "SubsurfaceScattering" || key.starts_with("Texture"))
                        {
                            continue;  // Parametri deja tratați
                        }

                        /*if (value->IsType<float>())*/
                        /*    material->SetParameter(key, value->GetData<float>());*/
                        /*else if (value->IsType<glm::vec2>())*/
                        /*    material->SetParameter(key, value->GetData<glm::vec2>());*/
                        /*else if (value->IsType<glm::vec3>())*/
                        /*    material->SetParameter(key, value->GetData<glm::vec3>());*/
                        /*else if (value->IsType<glm::vec4>())*/
                        /*    material->SetParameter(key, value->GetData<glm::vec4>());*/
                        /*else if (value->IsType<int>())*/
                        /*    material->SetParameter(key, value->GetData<int>());*/
                        /*else if (value->IsType<bool>())*/
                        /*    material->SetParameter(key, value->GetData<bool>());*/

                        if (value->GetTypeInfo() == "Float")
                        {
                            auto floatEntry =
                                std::dynamic_pointer_cast<Config::TypedEntry<float>>(value);
                            if (floatEntry)
                            {
                                material->SetParameter(key, floatEntry->GetData());
                            }
                            else
                            {
                                LOG_ERROR("Failed to cast entry to TypedEntry<float>");
                            }
                        }
                        else if (value->GetTypeInfo() == "Vec2")
                        {
                            auto vec2Entry =
                                std::dynamic_pointer_cast<Config::TypedEntry<glm::vec2>>(value);
                            if (vec2Entry)
                            {
                                material->SetParameter(key, vec2Entry->GetData());
                            }
                            else
                            {
                                LOG_ERROR("Failed to cast entry to TypedEntry<glm::vec2>");
                            }
                        }
                        else if (value->GetTypeInfo() == "Vec3")
                        {
                            auto vec3Entry =
                                std::dynamic_pointer_cast<Config::TypedEntry<glm::vec3>>(value);
                            if (vec3Entry)
                            {
                                material->SetParameter(key, vec3Entry->GetData());
                            }
                            else
                            {
                                LOG_ERROR("Failed to cast entry to TypedEntry<glm::vec3>");
                            }
                        }
                        else if (value->GetTypeInfo() == "Vec4")
                        {
                            auto vec4Entry =
                                std::dynamic_pointer_cast<Config::TypedEntry<glm::vec4>>(value);
                            if (vec4Entry)
                            {
                                material->SetParameter(key, vec4Entry->GetData());
                            }
                            else
                            {
                                LOG_ERROR("Failed to cast entry to TypedEntry<glm::vec4>");
                            }
                        }
                        else if (value->GetTypeInfo() == "Int")
                        {
                            auto intEntry =
                                std::dynamic_pointer_cast<Config::TypedEntry<int>>(value);
                            if (intEntry)
                            {
                                material->SetParameter(key, intEntry->GetData());
                            }
                            else
                            {
                                LOG_ERROR("Failed to cast entry to TypedEntry<int>");
                            }
                        }
                        else if (value->GetTypeInfo() == "Bool")
                        {
                            auto boolEntry =
                                std::dynamic_pointer_cast<Config::TypedEntry<bool>>(value);
                            if (boolEntry)
                            {
                                material->SetParameter(key, boolEntry->GetData());
                            }
                            else
                            {
                                LOG_ERROR("Failed to cast entry to TypedEntry<bool>");
                            }
                        }
                        else
                        {
                            LOG_WARN("Unknown parameter type: {}", value->GetTypeInfo());
                        }
                    }

                    // Adăugăm materialul în manager
                    materialComponents[material->Name] = material;
                    LOG_INFO("Material '{}' deserialized successfully from '{}'.",
                             material->Name,
                             entry.path().string());
                }
                catch (const std::exception& ex)
                {
                    LOG_ERROR("Failed to deserialize material '{}': {}",
                              entry.path().string(),
                              ex.what());
                }
            }
        }
    }

private:
    mutable std::mutex mutex_;  // Mutex for thread safety
    std::unordered_map<std::string, std::shared_ptr<Material>> materialComponents;
};

}  // namespace Forge


#endif
