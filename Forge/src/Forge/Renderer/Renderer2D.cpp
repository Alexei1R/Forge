// Renderer2D.cpp
#include "Renderer2D.h"
#include "Forge/Renderer/Buffer/BufferImpl.h"
#include "glm/gtc/matrix_transform.hpp"

namespace Forge {

Renderer2D::Renderer2D()
{
    Init();
}

Renderer2D::~Renderer2D()
{
    Shutdown();
}


void Renderer2D::Init()
{
    m_VertexArray = std::make_shared<VertexArrayBuffer>();

    m_VertexBuffer = std::make_shared<VertexBuffer>(
        nullptr,
        MaxVertices * sizeof(QuadVertex),
        VertexBufferDrawMode::Dynamic);

    BufferLayout layout = {{BufferDataType::Float3, "a_Position"},
                           {BufferDataType::Float4, "a_Color"}};
    m_VertexBuffer->SetLayout(layout);
    m_VertexArray->AddVertexBuffer(m_VertexBuffer);

    // Corrected indices generation
    uint32_t* quadIndices = new uint32_t[MaxIndices];
    uint32_t offset = 0;
    for (uint32_t i = 0; i < MaxIndices; i += 6)
    {
        quadIndices[i + 0] = offset + 0;
        quadIndices[i + 1] = offset + 1;
        quadIndices[i + 2] = offset + 3;

        quadIndices[i + 3] = offset + 1;
        quadIndices[i + 4] = offset + 2;
        quadIndices[i + 5] = offset + 3;

        offset += 4;
    }
    std::shared_ptr<IndexBuffer> quadIB = std::make_shared<IndexBuffer>(quadIndices, MaxIndices);
    m_VertexArray->SetIndexBuffer(quadIB);
    delete[] quadIndices;

    m_QuadVertexBufferBase = new QuadVertex[MaxVertices];

    // Load shader
    m_Shader = std::make_shared<Shader>(std::initializer_list<ShaderElement> {
        {ShaderType::VERTEX, "Assets/Shaders/Renderer2DQuad.vert"},
        {ShaderType::FRAGMENT, "Assets/Shaders/Renderer2DQuad.frag"}});
    m_Shader->Build();
}


void Renderer2D::Shutdown()
{
    delete[] m_QuadVertexBufferBase;
    m_QuadVertexBufferBase = nullptr;
    m_QuadVertexBufferPtr = nullptr;

    m_VertexArray.reset();
    m_VertexBuffer.reset();
    m_Shader.reset();
}

void Renderer2D::BeginScene(const std::shared_ptr<Camera>& camera)
{
    m_ViewProjectionMatrix = camera->GetViewProjectionMatrix();
    m_QuadVertexBufferPtr = m_QuadVertexBufferBase;
}

void Renderer2D::EndScene()
{
    size_t dataSize = (uint8_t*)m_QuadVertexBufferPtr - (uint8_t*)m_QuadVertexBufferBase;
    m_VertexBuffer->Bind();
    if (dataSize > 0)
    {
        glBufferSubData(GL_ARRAY_BUFFER, 0, dataSize, m_QuadVertexBufferBase);
    }
    Flush();
}

void Renderer2D::Flush()
{
    if (m_QuadIndexCount == 0)
        return;

    m_Shader->Bind();
    m_Shader->SetUniform("u_ViewProjection", m_ViewProjectionMatrix);

    m_VertexArray->Bind();
    glDrawElements(GL_TRIANGLES, m_QuadIndexCount, GL_UNSIGNED_INT, nullptr);
    m_QuadIndexCount = 0;
}

void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
{
    if (m_QuadIndexCount >= MaxIndices)
        EndScene();

    glm::vec3 positions[4];
    positions[0] = {position.x, position.y, 0.0f};  // Bottom-left (v0)
    positions[1] = {position.x + size.x, position.y, 0.0f};  // Bottom-right (v1)
    positions[2] = {position.x + size.x, position.y + size.y, 0.0f};  // Top-right (v2)
    positions[3] = {position.x, position.y + size.y, 0.0f};  // Top-left (v3)

    m_QuadVertexBufferPtr->Position = positions[0];
    m_QuadVertexBufferPtr->Color = color;
    m_QuadVertexBufferPtr++;

    m_QuadVertexBufferPtr->Position = positions[1];
    m_QuadVertexBufferPtr->Color = color;
    m_QuadVertexBufferPtr++;

    m_QuadVertexBufferPtr->Position = positions[2];
    m_QuadVertexBufferPtr->Color = color;
    m_QuadVertexBufferPtr++;

    m_QuadVertexBufferPtr->Position = positions[3];
    m_QuadVertexBufferPtr->Color = color;
    m_QuadVertexBufferPtr++;

    m_QuadIndexCount += 6;
}

}  // namespace Forge
