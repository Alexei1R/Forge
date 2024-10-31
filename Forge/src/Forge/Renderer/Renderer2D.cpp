//
// Created by toor on 2024-10-29.
//

#include "Renderer2D.h"
#include "Forge/Core/Log/Log.h"
#include "Forge/Renderer/BatchManager.h"
#include "Forge/Renderer/RenderCommand.h"
#include "Forge/Renderer/RenderableTargets/Quad.h"
#include <memory>

namespace Forge {

Renderer2D::Renderer2D() : m_Quad()
{
    m_Shader = std::make_shared<Shader>(
        ShaderLayout {{ShaderType::VERTEX, "Assets/Shaders/Renderer2DQuad.vert"},
                      {ShaderType::FRAGMENT, "Assets/Shaders/Renderer2DQuad.frag"}});
    m_Shader->Build();


    BatchProps props(100, m_Quad.GetLayout());
    m_QuadBatch = std::make_unique<BatchManager>(props);


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
    m_QuadBatch->EndBatch();
    m_Shader->Bind();
    m_Shader->SetUniform("u_ViewProjection", m_ViewProjectionMatrix);
    m_QuadBatch->Flush();
}


void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
{
    m_QuadBatch->Submit(m_Quad.GetVertices(position, size, color), m_Quad.GetIndices());
}

}  // namespace Forge
