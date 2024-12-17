#include "RendererBatch.h"

namespace Forge {

void RendererBatch::Init(uint32_t vertexSize, uint32_t indexSize, const BufferLayout& layout) {
    m_VertexStackSizeInBytes = 0;
    for (const auto& element : layout) {
        m_VertexStackSizeInBytes += element.size;
    }

    m_VerticesBytes.reserve(vertexSize * m_VertexStackSizeInBytes);
    m_Indices.reserve(indexSize);

    m_VAO = std::make_shared<VertexArrayBuffer>();
    m_VBO = std::make_shared<VertexBuffer>(nullptr, vertexSize * m_VertexStackSizeInBytes, BufferDrawMode::Dynamic);
    m_EBO = std::make_shared<IndexBuffer>(nullptr, indexSize * sizeof(uint32_t), BufferDrawMode::Dynamic);

    m_VAO->Bind();
    m_VBO->Bind();
    m_VBO->SetLayout(layout);
    m_VAO->AddVertexBuffer(m_VBO);
    m_EBO->Bind();
    m_VAO->SetIndexBuffer(m_EBO);
    m_VAO->Unbind();
}

void RendererBatch::Submit(const std::vector<MeshVertex>& vertices, const std::vector<uint32_t>& indices,
                           const std::shared_ptr<Material>& material) {
    if (m_CurrentMaterial != material) {
        Flush();
        m_CurrentMaterial = material;
    }

    if (!m_CurrentMaterial || m_CurrentMaterial->ShaderHandle.GetValue() == 0) {
        LOG_ERROR("Material or ShaderHandle is null");
        return;
    }

    m_VerticesBytes.insert(m_VerticesBytes.end(), reinterpret_cast<const char*>(vertices.data()),
                           reinterpret_cast<const char*>(vertices.data() + vertices.size()));

    for (const auto& index : indices) {
        m_Indices.push_back(index + m_CurrentSubmitCount);
    }

    m_CurrentSubmitCount += static_cast<uint32_t>(vertices.size());
}

void RendererBatch::Flush() {
    if (m_VerticesBytes.empty()) {
        return;
    }

    auto& shaderManager = ShaderManager::GetInstance();
    auto shader = shaderManager.GetShader(m_CurrentMaterial->ShaderHandle);
    shader->Bind();

    shader->SetUniform("Color", m_CurrentMaterial->Color);
    shader->SetUniform("EmissiveColor", m_CurrentMaterial->EmissiveColor);
    shader->SetUniform("Metallic", m_CurrentMaterial->Metallic);
    shader->SetUniform("Roughness", m_CurrentMaterial->Roughness);
    shader->SetUniform("Specular", m_CurrentMaterial->Specular);

    for (const auto& [name, value] : m_CurrentMaterial->AdditionalParameters) {
        std::visit(
            [&](auto&& arg) {
                shader->SetUniform(name, arg);
            },
            value);
    }

    auto& textureManager = TextureManager::GetInstance();
    for (size_t i = 0; i < m_CurrentMaterial->TextureHandles.size(); ++i) {
        auto textureHandle = m_CurrentMaterial->TextureHandles[i];
        if (textureHandle.GetValue() > 0) {
            auto texture = textureManager.GetTexture(textureHandle);
            texture->Bind(static_cast<uint32_t>(i));
        }
    }

    RenderCommand::DrawIndexed(m_VAO, static_cast<uint32_t>(m_Indices.size()));
    m_VAO->Unbind();
}

void RendererBatch::Begin() {
    m_VAO->Bind();
    m_VBO->SubmitData(m_VerticesBytes.data(), static_cast<uint32_t>(m_VerticesBytes.size()));
    m_EBO->SubmitData(m_Indices.data(), static_cast<uint32_t>(m_Indices.size() * sizeof(uint32_t)));
    m_CurrentSubmitCount = 0;
}

void RendererBatch::End() {
    m_VerticesBytes.clear();
    m_Indices.clear();
    m_CurrentSubmitCount = 0;
}

} // namespace Forge
