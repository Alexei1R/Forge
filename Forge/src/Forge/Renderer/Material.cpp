

#include "Material.h"

#include "RenderCommand.h"

namespace Forge {

Material::Material(const Handle& shaderHandle)
    : ShaderHandle(shaderHandle) {
    TextureHandles.resize(RenderCommand::GetMaxTextureSlots());
}

void Material::SetTexture(uint32_t slot, const Handle& textureHandle) {
    if (textureHandle == Handle(0)) {
        LOG_CRITICAL("Invalid textureHandle ")
        return;
    }

    if (slot < TextureHandles.size()) {
        TextureHandles[slot] = textureHandle;
    } else {
        LOG_ERROR("Texture slot {} exceeds maximum of texture slots", slot);
    }
}

uint32_t Material::GetTexureSlot(Handle handle) {
    for (uint32_t i = 0; i < TextureHandles.size(); ++i) {
        if (TextureHandles[i] == handle) {
            return i;
        }
    }
    LOG_WARN("Handle not found in TextureHandles.");
    return UINT32_MAX; // Cod de eroare dacă nu este găsit
}

void Material::Serialize() {
    Config::Config config;

    // Salvăm proprietățile de bază ale materialului
    config.AddEntry("Color", Color);
    config.AddEntry("EmissiveColor", EmissiveColor);
    config.AddEntry("Metallic", Metallic);
    config.AddEntry("Roughness", Roughness);
    config.AddEntry("Specular", Specular);
    config.AddEntry("ShaderHandle", (int)ShaderHandle.GetValue());

    // Salvăm identificatorii texturilor
    for (size_t i = 0; i < TextureHandles.size(); ++i) {
        if (TextureHandles[i] != Handle(0)) {
            config.AddEntry<int>("Texture" + std::to_string(i), TextureHandles[i].GetValue());
        }
    }

    // Salvăm parametrii suplimentari
    for (const auto& [key, value] : AdditionalParameters) {
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

    if (config.SaveToFile(file_path.string())) {
        LOG_INFO("Material '{}' saved successfully to '{}'.", Name, file_path.string());
    } else {
        LOG_ERROR("Failed to save material '{}' to '{}'.", Name, file_path.string());
    }
}

// NOTE: MaterialManager

std::shared_ptr<Material> MaterialManager::CreateMaterial(const std::string& name, const Handle& handle) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (materialComponents.find(name) != materialComponents.end()) {
        return materialComponents[name];
    }

    auto material = std::make_shared<Material>(handle);
    material->Name = name;
    materialComponents[name] = material;
    LOG_INFO("Material '{}' created and added to MaterialManager.", name);
    return material;
}

std::shared_ptr<Material> MaterialManager::GetMaterial(const std::string& name) const {
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = materialComponents.find(name);
    if (it != materialComponents.end()) {
        return it->second;
    }
    LOG_ERROR("Material '{}' not found in MaterialManager.", name);
    return nullptr;
}

void MaterialManager::RemoveMaterial(const std::string& name) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (!materialComponents.erase(name)) {
        LOG_WARN("Material '{}' not found in MaterialManager.", name);
    }
}

void MaterialManager::ClearAll() {
    std::lock_guard<std::mutex> lock(mutex_);
    materialComponents.clear();
}

void MaterialManager::Serialize() {
    for (auto& material : materialComponents) {
        material.second->Serialize();
    }
}

void MaterialManager::Deserialize() {
    std::lock_guard<std::mutex> lock(mutex_);

    std::filesystem::path current_path = std::filesystem::current_path();
    std::filesystem::path material_folder = current_path / "Assets/Material";

    if (!std::filesystem::exists(material_folder)) {
        LOG_ERROR("Material folder does not exist: {}", material_folder.string());
        return;
    }

    // Iterăm prin toate fișierele din folder
    for (const auto& entry : std::filesystem::directory_iterator(material_folder)) {
        if (entry.is_regular_file() && entry.path().extension() == ".mat") {
            Config::Config config;
            if (!config.LoadFromFile(entry.path().string())) {
                LOG_ERROR("Failed to load material file: {}", entry.path().string());
                continue;
            }

            auto material = std::make_shared<Material>(Handle(0));
            material->Name = entry.path().stem().string();
            LOG_ERROR("Loading Material {}", entry.path().stem().string())

            try {
                // Reconstruim proprietățile de bază
                material->Color = config.GetEntry<glm::vec4>("Color")->GetData();
                material->EmissiveColor = config.GetEntry<glm::vec3>("EmissiveColor")->GetData();
                material->Metallic = config.GetEntry<float>("Metallic")->GetData();
                material->Roughness = config.GetEntry<float>("Roughness")->GetData();
                material->Specular = config.GetEntry<float>("Specular")->GetData();

                material->ShaderHandle = Handle(config.GetEntry<int>("ShaderHandle")->GetData());

                // Reconstruim identificatorii texturilor
                for (size_t i = 0; i < material->TextureHandles.size(); ++i) {
                    auto textureKey = "Texture" + std::to_string(i);
                    if (auto entry = config.GetEntry<int>(textureKey)) {
                        Handle textureHandle = Handle(entry->GetData());
                        if (textureHandle != Handle(0)) {
                            material->TextureHandles[i] = textureHandle;
                        }
                    }
                }

                // Reconstruim parametrii suplimentari
                for (const auto& [key, value] : config.entries()) {
                    if (key == "Color" || key == "EmissiveColor" || key == "Metallic" || key == "Roughness" || key == "Specular" ||
                        key == "Opacity" || key == "RefractionIndex" || key == "Anisotropy" || key == "SubsurfaceScattering" ||
                        key.starts_with("Texture")) {
                        continue; // Parametri deja tratați
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

                    if (value->GetTypeInfo() == "Float") {
                        auto floatEntry = std::dynamic_pointer_cast<Config::TypedEntry<float>>(value);
                        if (floatEntry) {
                            material->SetParameter(key, floatEntry->GetData());
                        } else {
                            LOG_ERROR("Failed to cast entry to TypedEntry<float>");
                        }
                    } else if (value->GetTypeInfo() == "Vec2") {
                        auto vec2Entry = std::dynamic_pointer_cast<Config::TypedEntry<glm::vec2>>(value);
                        if (vec2Entry) {
                            material->SetParameter(key, vec2Entry->GetData());
                        } else {
                            LOG_ERROR("Failed to cast entry to TypedEntry<glm::vec2>");
                        }
                    } else if (value->GetTypeInfo() == "Vec3") {
                        auto vec3Entry = std::dynamic_pointer_cast<Config::TypedEntry<glm::vec3>>(value);
                        if (vec3Entry) {
                            material->SetParameter(key, vec3Entry->GetData());
                        } else {
                            LOG_ERROR("Failed to cast entry to TypedEntry<glm::vec3>");
                        }
                    } else if (value->GetTypeInfo() == "Vec4") {
                        auto vec4Entry = std::dynamic_pointer_cast<Config::TypedEntry<glm::vec4>>(value);
                        if (vec4Entry) {
                            material->SetParameter(key, vec4Entry->GetData());
                        } else {
                            LOG_ERROR("Failed to cast entry to TypedEntry<glm::vec4>");
                        }
                    } else if (value->GetTypeInfo() == "Int") {
                        auto intEntry = std::dynamic_pointer_cast<Config::TypedEntry<int>>(value);
                        if (intEntry) {
                            material->SetParameter(key, intEntry->GetData());
                        } else {
                            LOG_ERROR("Failed to cast entry to TypedEntry<int>");
                        }
                    } else if (value->GetTypeInfo() == "Bool") {
                        auto boolEntry = std::dynamic_pointer_cast<Config::TypedEntry<bool>>(value);
                        if (boolEntry) {
                            material->SetParameter(key, boolEntry->GetData());
                        } else {
                            LOG_ERROR("Failed to cast entry to TypedEntry<bool>");
                        }
                    } else {
                        LOG_WARN("Unknown parameter type: {}", value->GetTypeInfo());
                    }
                }

                // Adăugăm materialul în manager
                materialComponents[material->Name] = material;
                LOG_INFO("Material '{}' deserialized successfully from '{}'.", material->Name, entry.path().string());
            } catch (const std::exception& ex) {
                LOG_ERROR("Failed to deserialize material '{}': {}", entry.path().string(), ex.what());
            }
        }
    }
}

} // namespace Forge
