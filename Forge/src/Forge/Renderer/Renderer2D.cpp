//
// Created by toor on 2024-10-29.
//

#include "Renderer2D.h"
#include "Forge/Renderer/RenderableTargets/Quad.h"

namespace Forge {

Renderer2D::Renderer2D() : m_QuadBatch(1)
{
    // NOTE: Begin
    Quad quad;
    m_QuadBatch.BeginBatch();

    m_QuadBatch.Submit<QuadVertex>(quad.GetVertices(), quad.GetIndices());
    m_QuadBatch.Submit<QuadVertex>(quad.GetVertices(), quad.GetIndices());
    m_QuadBatch.Submit<QuadVertex>(quad.GetVertices(), quad.GetIndices());
    m_QuadBatch.Submit<QuadVertex>(quad.GetVertices(), quad.GetIndices());
    m_QuadBatch.Submit<QuadVertex>(quad.GetVertices(), quad.GetIndices());
    m_QuadBatch.Submit<QuadVertex>(quad.GetVertices(), quad.GetIndices());
    m_QuadBatch.Submit<QuadVertex>(quad.GetVertices(), quad.GetIndices());
    m_QuadBatch.EndBatch();

    m_QuadBatch.Flush();
}

Renderer2D::~Renderer2D() {}


void Renderer2D::BeginScene(const std::shared_ptr<Camera>& camera)
{
    m_Camera = camera;
    m_ViewProjectionMatrix = camera->GetViewProjectionMatrix();
}

void Renderer2D::EndScene() {}


void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
{
}

}  // namespace Forge
