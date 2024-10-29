//
// Created by toor on 2024-10-29.
//

#include "Renderer2D.h"
#include "Forge/Renderer/RenderableTargets/Quad.h"

namespace Forge {

Renderer2D::Renderer2D() : m_QuadBatch(10), m_Quad() {}

Renderer2D::~Renderer2D() {}


void Renderer2D::BeginScene(const std::shared_ptr<Camera>& camera)
{
    m_Camera = camera;
    m_ViewProjectionMatrix = camera->GetViewProjectionMatrix();

    m_QuadBatch.BeginBatch();
}

void Renderer2D::EndScene()
{
    m_QuadBatch.EndBatch();

    m_QuadBatch.Flush();
}


void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
{
    m_QuadBatch.Submit(m_Quad.GetVertices(), m_Quad.GetIndices());
}

}  // namespace Forge
