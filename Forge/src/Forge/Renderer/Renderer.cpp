

#include "Renderer.h"
#include "Forge/Core/Log/Log.h"
#include "Forge/Core/Utils.h"
#include "Forge/Core/Hash.h"
#include "Forge/Renderer/Mesh.h"
#include "glm/fwd.hpp"

namespace Forge {

bool Renderer::m_IsInScope = false;
bool Renderer::m_IsInitialized = false;
std::unique_ptr<UniformBuffer> Renderer::m_UniformBuffer = nullptr;
std::unordered_map<uint32_t, RendererBatch> Forge::Renderer::m_RenderBatches;

// UI Precalculated Size
uint32_t Renderer::m_PrecalculatedVerticesSize = 1000;
uint32_t Renderer::m_PrecalculatedIndicesSize = m_PrecalculatedVerticesSize * 4;

bool Renderer::Initialize()
{
    if (m_IsInitialized)
    {
        LOG_CRITICAL("The Renderer context is already initialized")
        return false;
    }

    // NOTE: Here Begin Initialization
    m_UniformBuffer = std::make_unique<UniformBuffer>(sizeof(glm::mat4), 0);


    // NOTE: End
    m_IsInitialized = true;
    return true;
}
bool Renderer::Shutdown()
{
    if (!m_IsInitialized)
    {
        F_ASSERT(false, "The Renderer context is not initialized")
    }
    // NOTE: Here Begin Shutdown

    m_RenderBatches.clear();
    m_UniformBuffer.reset();

    // NOTE: End
    m_IsInitialized = false;
    return true;
}


void Renderer::CheckScope()
{
    if (!m_IsInitialized)
    {
        F_ASSERT(false, "The Renderer context is not initialized")
    }
    if (!m_IsInScope)
    {
        F_ASSERT(false, "The Submit function is not in scope of Begin/End")
    }
}


void Renderer::Begin(const std::shared_ptr<Camera>& camera)
{
    if (!m_IsInitialized)
    {
        F_ASSERT(false, "The Renderer context is not initialized")
    }
    if (m_IsInScope)
    {
        LOG_CRITICAL("Ensure that Renderer::End() is called before Renderer::Begin()")
        return;
    }
    m_IsInScope = true;
    // NOTE: Here Begin
    m_UniformBuffer->Submit(&camera->GetViewProjectionMatrix(), sizeof(glm::mat4));

    // NOTE: End
}


void Renderer::End()
{
    if (!m_IsInitialized)
    {
        F_ASSERT(false, "The Renderer context is not initialized")
    }
    if (!m_IsInScope)
    {
        LOG_CRITICAL("Ensure that Renderer::Begin() is called before Renderer::End()")
        return;
    }
    m_IsInScope = false;

    // NOTE: Here Begin Shutdown

    for (auto& [materialHash, batch] : m_RenderBatches)
    {
        batch.Flush();
    }
    // NOTE: End
}


void Renderer::SubmitMesh(const MeshTarget& target)
{
    // Get materials and meshes from the target
    const auto& materials = target.GetMaterials();
    const auto& meshes = target.GetMeshes();

    // Iterate over each mesh
    for (const auto& mesh : meshes)
    {
        // Get the material for the current mesh
        uint32_t materialIndex = mesh.MaterialIndex;
        if (materialIndex >= materials.size())
        {
            LOG_WARN("Invalid material index for mesh.");
            return;
        }

        const auto& material = materials[materialIndex];

        // Generate a hash for the material to group meshes that share the same material
        uint8_t materialData[sizeof(material)];
        std::memcpy(materialData, &material, sizeof(material));
        uint32_t materialHash = HashFast::GenerateU32BaseHash(materialData, sizeof(materialData));

        // Check if a batch for this material hash exists, if not create a new batch
        if (m_RenderBatches.find(materialHash) == m_RenderBatches.end())
        {
            auto& batch = m_RenderBatches[materialHash];
            batch.Init(1000, 1000, target.GetLayout());

            std::vector<uint8_t> m_VerticesBytes;
            auto& vertices = mesh.Vertices;
            m_VerticesBytes.resize(vertices.size() * sizeof(MeshVertex));
            memcpy(m_VerticesBytes.data(), vertices.data(), m_VerticesBytes.size());
            batch.Submit(m_VerticesBytes, mesh.Indices, std::make_shared<Material>(material));
        }
        else
        {
            auto& batch = m_RenderBatches[materialHash];
            std::vector<uint8_t> m_VerticesBytes;
            auto& vertices = mesh.Vertices;
            m_VerticesBytes.resize(vertices.size() * sizeof(MeshVertex));
            memcpy(m_VerticesBytes.data(), vertices.data(), m_VerticesBytes.size());
            batch.Submit(m_VerticesBytes, mesh.Indices, std::make_shared<Material>(material));
        }
    }
}


void Renderer::SubmitUIElement(bf::Widget& widget)
{
    const auto& material = widget.GetMaterial();

    uint8_t materialData[sizeof(material)];
    std::memcpy(materialData, &material, sizeof(material));
    uint32_t materialHash = HashFast::GenerateU32BaseHash(materialData, sizeof(materialData));

    // Check if batch exists, if not create it
    if (m_RenderBatches.find(materialHash) == m_RenderBatches.end())
    {
        auto& batch = m_RenderBatches[materialHash];

        batch.Init(m_PrecalculatedVerticesSize, m_PrecalculatedIndicesSize, widget.GetLayout());
        auto& [vertices, indices] = widget.GetDrawList();
        batch.Submit(vertices, indices, material);
    }
    else
    {
        auto& batch = m_RenderBatches[materialHash];
        auto& [vertices, indices] = widget.GetDrawList();
        batch.Submit(vertices, indices, material);
    }
}


void Renderer::SetPrecalculatedUIElementSize(uint32_t vertices, uint32_t indices)
{
    m_PrecalculatedVerticesSize = vertices;
    m_PrecalculatedVerticesSize = indices;
}

}  // namespace Forge
