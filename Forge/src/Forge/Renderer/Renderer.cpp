

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

    m_MeshBatch.clear();
    m_UIBatch.clear();
    m_UniformBuffer.reset();
    return true;
}

void Renderer::Begin() {
    if (m_IsInScope) {
        LOG_CRITICAL("Ensure that Renderer::End() is called before Renderer::Begin()")
        return;
    }
    m_IsInScope = true;
}

void Renderer::End() {

    if (!m_IsInScope) {
        LOG_CRITICAL("Ensure that Renderer::Begin() is called before Renderer::End()")
        return;
    }
    m_IsInScope = false;
}

void Renderer::Submit() {

    if (m_NeedSubmit) {

        for (auto& [_, batch] : m_MeshBatch) {
            batch.Begin();
        }

        for (auto& [_, batch] : m_UIBatch) {
            batch.Begin();
        }
        m_NeedSubmit = false;
    }
}
void Renderer::DrawUI(const std::shared_ptr<Camera>& camera) {

    if (!m_IsInScope) {
        F_ASSERT(false, "The Submit function is not in scope of Begin/End")
    }
    m_UniformBuffer->Submit(&camera->GetViewProjectionMatrix(), sizeof(glm::mat4));

    // Draw UI
    for (auto& [materialHash, batch] : m_UIBatch) {
        batch.Flush();
    }
}

void Renderer::DrawMesh(const std::shared_ptr<Camera>& camera) {

    if (!m_IsInScope) {
        F_ASSERT(false, "The Submit function is not in scope of Begin/End")
    }

    m_UniformBuffer->Submit(&camera->GetViewProjectionMatrix(), sizeof(glm::mat4));

    // Draw Mesh
    for (auto& [materialHash, batch] : m_MeshBatch) {
        batch.Flush();
    }
}
void Renderer::AddMesh(const MeshTarget& target) {

    const auto& materials = target.GetMaterials();
    const auto& meshes = target.GetMeshes();

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
        if (m_MeshBatch.find(materialHash) == m_MeshBatch.end()) {

            auto& batch = m_MeshBatch[materialHash];
            batch.Init(BATCH_VERTEX_SIZE, BATCH_INDEX_SIZE, target.GetLayout());
        }

        auto& batch = m_MeshBatch[materialHash];
        batch.Submit(mesh.Vertices, mesh.Indices, material);
    }
}

void Renderer::AddUI(bf::Widget& widget) {

    const auto& material = widget.GetMaterial();

    uint8_t materialData[sizeof(material)];
    std::memcpy(materialData, &material, sizeof(material));
    uint32_t materialHash = HashFast::GenerateU32BaseHash(materialData, sizeof(materialData));

    // Check if batch exists, if not create it
    if (m_UIBatch.find(materialHash) == m_UIBatch.end()) {
        auto& batch = m_UIBatch[materialHash];

        batch.Init(BATCH_VERTEX_SIZE, BATCH_INDEX_SIZE, widget.GetLayout());
        auto& [vertices, indices] = widget.GetDrawList();

        for (auto singleVertex : vertices) {
            LOG_ERROR("{} {}", singleVertex.Position.x, singleVertex.Position.y)
        }

        for (auto index : indices) {
            LOG_ERROR("{}", index)
        }
        batch.Submit(vertices, indices, material);
    } else {
        auto& batch = m_UIBatch[materialHash];
        auto& [vertices, indices] = widget.GetDrawList();
        batch.Submit(vertices, indices, material);
    }
}

} // namespace Forge
