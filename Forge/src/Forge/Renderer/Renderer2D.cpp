//
// Created by toor on 2024-10-29.
//

#include "Renderer2D.h"
#include "Forge/Core/Log/Log.h"
#include "Forge/Renderer/BatchManager.h"
#include "Forge/Renderer/Buffer/BufferImpl.h"
#include "Forge/Renderer/RenderCommand.h"
#include "Forge/Renderer/RenderableTargets/Quad.h"
#include <memory>

namespace Forge {

Renderer2D::Renderer2D()
{
    m_Shader = std::make_shared<Shader>(
        ShaderLayout {{ShaderType::VERTEX, "Assets/Shaders/Renderer2DQuad.vert"},
                      {ShaderType::FRAGMENT, "Assets/Shaders/Renderer2DQuad.frag"}});
    m_Shader->Build();

    glm::mat4 transform = glm::mat4(1.0);
    glm::vec4 color = glm::vec4(1.0, 0.0, 1.0, 1.0);

    m_Vertices = {{glm::vec3(-0.5f, -0.5f, 0.0f), color},
                  {glm::vec3(0.5f, -0.5f, 0.0f), color},
                  {glm::vec3(0.5f, 0.5f, 0.0f), color},
                  {glm::vec3(-0.5f, 0.5f, 0.0f), color}};


    m_Indices = {0, 1, 2, 2, 3, 0};


    m_VAO = std::make_shared<VertexArrayBuffer>();
    m_VBO = std::make_shared<VertexBuffer>(nullptr,
                                           m_Vertices.size() * sizeof(QuadVertex),
                                           BufferDrawMode::Dynamic);
    m_EBO = std::make_shared<IndexBuffer>(nullptr,
                                          m_Indices.size() * sizeof(uint32_t),
                                          BufferDrawMode::Dynamic);


    m_VAO->Bind();
    m_VBO->Bind();


    BufferLayout m_Layout = {{BufferDataType::Float3, "a_Position"},
                             {BufferDataType::Float4, "a_Color"}};
    m_VBO->SetLayout(m_Layout);
    m_VAO->AddVertexBuffer(m_VBO);

    m_EBO->Bind();
    m_VAO->SetIndexBuffer(m_EBO);

    m_VAO->Unbind();


    LOG_INFO("MaxTextureSlots {}", RenderCommand::GetMaxTextureSlots())
}

Renderer2D::~Renderer2D() {}


void Renderer2D::BeginScene(const std::shared_ptr<Camera>& camera)
{
    m_Camera = camera;
    m_ViewProjectionMatrix = camera->GetViewProjectionMatrix();
}

void Renderer2D::EndScene()
{
    m_VAO->Bind();

    m_Shader->Bind();
    m_Shader->SetUniform("u_ViewProjection", m_ViewProjectionMatrix);
    // Upload vertex data
    m_VBO->Bind();
    m_VBO->SubmitData(m_Vertices.data(), m_Vertices.size() * sizeof(QuadVertex));

    m_EBO->Bind();
    m_EBO->SubmitData(m_Indices.data(), m_Indices.size() * sizeof(uint32_t));


    RenderCommand::Draw(m_VAO);
}

void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
{
}

}  // namespace Forge
