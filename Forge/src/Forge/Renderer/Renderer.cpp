

#include "Renderer.h"

#include "Forge/Core/Hash.h"
#include "Forge/Core/Log/Log.h"
#include "Forge/Core/Utils.h"
#include "Forge/Renderer/Mesh.h"
#include "glm/fwd.hpp"

namespace Forge {

Renderer::Renderer() {
    Init();
}
Renderer::~Renderer() {
    Shutdown();
}

void Renderer::Init() {
    m_UniformBuffer = std::make_unique<UniformBuffer>(sizeof(glm::mat4), 0);
}
bool Renderer::Shutdown() {

    m_RenderBatches.clear();
    m_UniformBuffer.reset();
    return true;
}

void Renderer::Begin() {
    if (m_IsInScope) {
        LOG_CRITICAL("Ensure that Renderer::End() is called before Renderer::Begin()")
        return;
    }
    m_IsInScope = true;
    for (auto& [_, batch] : m_RenderBatches) {
        batch.Begin();
    }
}

void Renderer::End() {

    if (!m_IsInScope) {
        LOG_CRITICAL("Ensure that Renderer::Begin() is called before Renderer::End()")
        return;
    }
    m_IsInScope = false;
}

void Renderer::Draw(const std::shared_ptr<Camera>& camera) {

    if (!m_IsInScope) {
        F_ASSERT(false, "The Submit function is not in scope of Begin/End")
    }
    m_IsInScope = true;
    m_UniformBuffer->Submit(&camera->GetViewProjectionMatrix(), sizeof(glm::mat4));

    for (auto& [materialHash, batch] : m_RenderBatches) {
        batch.Flush();
    }
}

void Renderer::AddMesh(const MeshTarget& target) {

    const auto& materials = target.GetMaterials();
    const auto& meshes = target.GetMeshes();

    uint32_t indicesOffset;

    for (const auto& mesh : meshes) {
        std::shared_ptr<Material> material;
        uint32_t materialIndex = mesh.MaterialIndex;

        if (materialIndex >= materials.size()) {
            material = MeshTarget::GetDefaultMaterial();
        } else {
            material = std::make_shared<Material>(materials[materialIndex]);
        }

        // NOTE: Generate a hash for the material to group meshes that share the same material
        uint8_t materialData[sizeof(Material)];
        std::memcpy(materialData, material.get(), sizeof(Material));
        // Material Hash
        uint32_t materialHash = HashFast::GenerateU32BaseHash(materialData, sizeof(materialData));

        // NOTE:Check if a batch for this material hash exists; if not, create a new batch
        if (m_RenderBatches.find(materialHash) == m_RenderBatches.end()) {

            auto& batch = m_RenderBatches[materialHash];
            batch.Init(BATCH_VERTEX_SIZE, BATCH_INDEX_SIZE, target.GetLayout());
            LOG_WARN("Creating batch")
        }

        auto& batch = m_RenderBatches[materialHash];
        batch.Submit(mesh.Vertices, mesh.Indices, material);
    }
}

} // namespace Forge
