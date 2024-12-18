

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

} // namespace Forge
