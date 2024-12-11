
#include "RendererBatch.h"
#include "Forge/Core/Log/Log.h"
#include "Forge/Renderer/ShaderManager.h"
#include "Forge/Renderer/TextureManager.h"
#include "Forge/Renderer/RenderCommand.h"

namespace Forge {


void RendererBatch::Init(const uint32_t vertexSize,
                         const uint32_t indexSize,
                         const BufferLayout& layout)
{
    // NOTE: Compute Vertex Type size
    uint32_t vsBytes = 0;
    for (auto& element : layout)
    {
        vsBytes += element.size;
    }
    m_VertexStackSizeInBytes = vsBytes;

    m_VerticesBytes.reserve(vertexSize * vsBytes);
    m_Indices.reserve(indexSize);

    m_VAO = std::make_shared<VertexArrayBuffer>();
    m_VBO = std::make_shared<VertexBuffer>(nullptr, vertexSize * vsBytes, BufferDrawMode::Dynamic);
    m_EBO = std::make_shared<IndexBuffer>(nullptr,
                                          indexSize * sizeof(uint32_t),
                                          BufferDrawMode::Dynamic);

    m_VAO->Bind();
    m_VBO->Bind();
    m_VBO->SetLayout(layout);
    m_VAO->AddVertexBuffer(m_VBO);
    m_EBO->Bind();
    m_VAO->SetIndexBuffer(m_EBO);
    m_VAO->Unbind();
}


void RendererBatch::Submit(const std::vector<uint8_t>& vertices,
                           const std::vector<uint32_t>& indices,
                           const std::shared_ptr<Material>& material)
{
    if (m_CurrentMaterial != material)
    {
        Flush();
        m_CurrentMaterial = material;
    }


    if (!m_CurrentMaterial || m_CurrentMaterial->ShaderHandle.GetValue() == 0)
    {
        LOG_ERROR("Material or ShaderHandle is null");
        return;
    }

    // NOTE: Copy vertex data
    m_VerticesBytes.insert(m_VerticesBytes.end(),
                           reinterpret_cast<const uint8_t*>(vertices.data()),
                           reinterpret_cast<const uint8_t*>(vertices.data() + vertices.size()));
    // NOTE: Copy index data
    for (auto& index : indices)
    {
        m_Indices.push_back(index + m_CurrentSubmitCount);
    }

    m_CurrentSubmitCount++;
}


void RendererBatch::Flush()
{
    if (m_VerticesBytes.empty())
        return;

    auto& shaderManager = ShaderManager::GetInstance();
    auto shader = shaderManager.GetShader(m_CurrentMaterial->ShaderHandle);
    shader->Bind();

    // Set Default Parameters
    shader->SetUniform("Color", m_CurrentMaterial->Color);
    shader->SetUniform("EmissiveColor", m_CurrentMaterial->EmissiveColor);
    shader->SetUniform("Metallic", m_CurrentMaterial->Metallic);
    shader->SetUniform("Roughness", m_CurrentMaterial->Roughness);
    shader->SetUniform("Specular", m_CurrentMaterial->Specular);

    for (const auto& [name, value] : m_CurrentMaterial->aditionlParameters)
    {
        std::visit(
            [&](auto&& arg) {
                shader->SetUniform(name, arg);
            },
            value);
    }

    auto& textureManager = TextureManager::GetInstance();
    for (size_t i = 0; i < m_CurrentMaterial->TextureHandles.size(); ++i)
    {
        auto textureHandle = m_CurrentMaterial->TextureHandles[i];
        if (textureHandle.GetValue() > 0)
        {
            auto texture = textureManager.GetTexture(textureHandle);
            texture->Bind(i);
        }
    }

    m_VAO->Bind();
    m_VBO->Bind();
    m_VBO->SubmitData(m_VerticesBytes.data(),
                      m_VerticesBytes.size() * sizeof(m_VertexStackSizeInBytes));
    m_EBO->Bind();
    m_EBO->SubmitData(m_Indices.data(), m_Indices.size() * sizeof(uint32_t));

    RenderCommand::DrawIndexed(m_VAO, m_Indices.size());

    m_VAO->Unbind();

    // Clear vectors without deallocation
    m_VerticesBytes.clear();
    m_Indices.clear();
    m_CurrentSubmitCount = 0;
}
}  // namespace Forge
