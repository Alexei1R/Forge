//
// Created by toor on 2024-10-29.
//

#include "Renderer2D.h"
#include "Forge/Core/Log/Log.h"
#include "Forge/Renderer/BatchManager.h"
#include "Forge/Renderer/RenderCommand.h"
#include "Forge/Renderer/RenderableTargets/Quad.h"
#include "glm/fwd.hpp"
#include <memory>

namespace Forge {

Renderer2D::Renderer2D()
{
    m_Shader = std::make_shared<Shader>(
        ShaderLayout {{ShaderType::VERTEX, "Assets/Shaders/Renderer2DQuad.vert"},
                      {ShaderType::FRAGMENT, "Assets/Shaders/Renderer2DQuad.frag"}});
    m_Shader->Build();


    BatchProps props(100,
                     m_Quad.GetVertices(glm::vec3(1.0), glm::vec2(1.0), glm::vec4(1.0)).size(),
                     m_Quad.GetIndices().size(),
                     m_Quad.GetLayout());
    m_QuadBatch = std::make_unique<BatchManager<QuadVertex>>(props);


    LOG_INFO("MaxTextureSlots {}", RenderCommand::GetMaxTextureSlots())
}

Renderer2D::~Renderer2D() {}


void Renderer2D::BeginScene(const std::shared_ptr<Camera>& camera)
{
    m_Camera = camera;
    m_ViewProjectionMatrix = camera->GetViewProjectionMatrix();


    m_QuadBatch->BeginBatch();
}

void Renderer2D::EndScene()
{
    m_Shader->Bind();
    m_Shader->SetUniform("u_ViewProjection", m_ViewProjectionMatrix);
    m_QuadBatch->EndBatch();
}


void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
{
    m_QuadBatch->Submit(m_Quad.GetVertices(position, size, color), m_Quad.GetIndices());
}

}  // namespace Forge
