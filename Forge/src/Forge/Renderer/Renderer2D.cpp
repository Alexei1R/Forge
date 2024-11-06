//
// Created by toor on 2024-10-29.
//

#include "Renderer2D.h"
#include "Forge/Core/Log/Log.h"
#include "Forge/Renderer/BatchManager.h"
#include "Forge/Renderer/RenderCommand.h"
#include "Forge/Renderer/RenderableTargets/Quad.h"
#include "glm/fwd.hpp"
#include <algorithm>
#include <memory>

namespace Forge {

Renderer2D::Renderer2D()
{
    // Slightly higher capacity to avoid frequent flushing
    BatchProps props(12000,
                     4,
                     6,
                     {{BufferDataType::Float3, "a_Position"}, {BufferDataType::Float4, "a_Color"}});
    m_QuadBatch = std::make_unique<BatchManager<QuadVertex>>(props);

    LOG_INFO("MaxTextureSlots {}", RenderCommand::GetMaxTextureSlots())
}

Renderer2D::~Renderer2D() {}

void Renderer2D::BeginScene(const std::shared_ptr<Camera>& camera)
{
    m_Camera = camera;
    m_ViewProjectionMatrix = camera->GetViewProjectionMatrix();
    m_QuadBatch->BeginBatch(m_ViewProjectionMatrix);
}

void Renderer2D::EndScene()
{
    m_QuadBatch->EndBatch();
}

void Renderer2D::DrawQuad(const glm::vec3& position,
                          const glm::vec2& size,
                          const std::shared_ptr<Material>& material)
{
    const auto color = material->GetParameter<glm::vec4>("Color");
    if (!color.has_value())
    {
        LOG_WARN("Color parameter not found or incorrect type");
        return;
    }

    // Precompute transformation matrix
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
                          glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

    // Static local-space vertices for a unit quad
    static const std::array<glm::vec4, 4> localVertices = {
        glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f),
        glm::vec4(0.5f, -0.5f, 0.0f, 1.0f),
        glm::vec4(0.5f, 0.5f, 0.0f, 1.0f),
        glm::vec4(-0.5f, 0.5f, 0.0f, 1.0f)};

    // Transform vertices and apply color
    std::vector<QuadVertex> transformedVertices;
    transformedVertices.reserve(4);
    for (const auto& localVertex : localVertices)
    {
        glm::vec3 worldPosition = glm::vec3(transform * localVertex);
        transformedVertices.push_back({worldPosition, color.value()});
    }

    // Predefined index order (two triangles for the quad)
    static const std::vector<uint32_t> quadIndices = {0, 1, 2, 2, 3, 0};

    m_QuadBatch->Submit(transformedVertices, quadIndices, material);
}

}  // namespace Forge
