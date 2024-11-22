//
// Created by toor on 2024-11-18.
//


#include "Renderer.h"
#include "Forge/Core/Log/Log.h"
#include "Forge/Core/Utils.h"
#include "Forge/Core/Hash.h"
#include "glm/fwd.hpp"

namespace Forge {

bool Renderer::m_IsInScope = false;
bool Renderer::m_IsInitialized = false;
std::unique_ptr<UniformBuffer> Renderer::m_UniformBuffer = nullptr;
std::unordered_map<uint32_t, RendererBatch> Forge::Renderer::m_RenderBatches;

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
    // NOTE: Here Begin Shutdown
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


void Renderer::SubmitMesh(const RenderableTarget& target, const std::shared_ptr<Material>& material)
{
    uint8_t materialData[sizeof(material)];
    std::memcpy(materialData, &material, sizeof(material));
    uint32_t materialHash = HashFast::GenerateU32BaseHash(materialData, sizeof(materialData));

    // Check if batch exists, if not create it
    if (m_RenderBatches.find(materialHash) == m_RenderBatches.end())
    {
        auto& batch = m_RenderBatches[materialHash];
        batch.Init(1000, 1000, target.GetLayout());
        batch.Submit(target.GetVertices(), target.GetIndices(), material);
    }
    else
    {
        auto& batch = m_RenderBatches[materialHash];
        batch.Submit(target.GetVertices(), target.GetIndices(), material);
    }
}

void Renderer::SubmitText(const RenderableTarget& target, const std::shared_ptr<Material>& material)
{
    uint8_t materialData[sizeof(material)];
    std::memcpy(materialData, &material, sizeof(material));
    uint32_t materialHash = HashFast::GenerateU32BaseHash(materialData, sizeof(materialData));

    // Check if batch exists, if not create it
    if (m_RenderBatches.find(materialHash) == m_RenderBatches.end())
    {
        auto& batch = m_RenderBatches[materialHash];
        batch.Init(1000, 1000, target.GetLayout());
        batch.Submit(target.GetVertices(), target.GetIndices(), material);
    }
    else
    {
        auto& batch = m_RenderBatches[materialHash];
        batch.Submit(target.GetVertices(), target.GetIndices(), material);
    }
}


void Renderer::SubmitUIElement(const BfUI::Widget& widget)
{
    const auto& material = widget.GetMaterial();

    uint8_t materialData[sizeof(material)];
    std::memcpy(materialData, &material, sizeof(material));
    uint32_t materialHash = HashFast::GenerateU32BaseHash(materialData, sizeof(materialData));

    // Check if batch exists, if not create it
    if (m_RenderBatches.find(materialHash) == m_RenderBatches.end())
    {
        auto& batch = m_RenderBatches[materialHash];
        batch.Init(1000, 1000, widget.GetLayout());
        batch.Submit(widget.GetVertices(), widget.GetIndices(), material);
    }
    else
    {
        auto& batch = m_RenderBatches[materialHash];
        batch.Submit(widget.GetVertices(), widget.GetIndices(), material);
    }
}
}  // namespace Forge
