#ifndef RENDERERBATCH_H
#define RENDERERBATCH_H

#include "Forge/Core/Log/Log.h"
#include "Forge/Renderer/Buffer/Buffer.h"
#include "Forge/Renderer/Buffer/BufferImpl.h"
#include "Forge/Renderer/Material.h"
#include "Forge/Renderer/RenderCommand.h"
#include "Forge/Renderer/ShaderManager.h"
#include "Forge/Renderer/TextureManager.h"
#include <memory>
#include <vector>

namespace Forge {

template <typename Vertex>
class RendererBatch {
public:
    RendererBatch() = default;
    ~RendererBatch() = default;

    RendererBatch(const RendererBatch&) = delete;
    RendererBatch& operator=(const RendererBatch&) = delete;
    RendererBatch(RendererBatch&&) noexcept = default;
    RendererBatch& operator=(RendererBatch&&) noexcept = default;

    void Init(uint32_t vertexSize, uint32_t indexSize, const BufferLayout& layout) {

        m_Vertices.reserve(vertexSize);
        m_Indices.reserve(indexSize);

        m_VAO = std::make_shared<VertexArrayBuffer>();
        m_VBO = std::make_shared<VertexBuffer>(nullptr, vertexSize * sizeof(Vertex), BufferDrawMode::Dynamic);
        m_EBO = std::make_shared<IndexBuffer>(nullptr, indexSize * sizeof(uint32_t), BufferDrawMode::Dynamic);

        m_VAO->Bind();
        m_VBO->Bind();
        m_VBO->SetLayout(layout);
        m_VAO->AddVertexBuffer(m_VBO);
        m_EBO->Bind();
        m_VAO->SetIndexBuffer(m_EBO);
        m_VAO->Unbind();
    }
    void Submit(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, const std::shared_ptr<Material>& material) {
        if (m_CurrentMaterial != material) {
            Flush();
            m_CurrentMaterial = material;
        }

        if (!m_CurrentMaterial || m_CurrentMaterial->ShaderHandle.GetValue() == 0) {
            LOG_ERROR("Invalid material or shader handle in RendererBatch::Flush()");
            return;
        }

        m_Vertices.insert(m_Vertices.end(), vertices.begin(), vertices.end());

        for (const auto& index : indices) {
            m_Indices.push_back(index + m_CurrentSubmitCount);
        }

        m_CurrentSubmitCount += static_cast<uint32_t>(vertices.size());
    }

    void Flush() {
        if (m_Vertices.empty()) {
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
    void Begin() {

        m_VAO->Bind();
        m_VBO->SubmitData(m_Vertices.data(), static_cast<uint32_t>(m_Vertices.size()) * sizeof(Vertex));
        m_EBO->SubmitData(m_Indices.data(), static_cast<uint32_t>(m_Indices.size() * sizeof(uint32_t)));
        m_CurrentSubmitCount = 0;
    }
    void End() {

        m_Vertices.clear();
        m_Indices.clear();
        m_CurrentSubmitCount = 0;
    }

private:
    void SetupBuffers();
    std::vector<Vertex> m_Vertices;
    std::vector<uint32_t> m_Indices;

    std::shared_ptr<VertexArrayBuffer> m_VAO;
    std::shared_ptr<VertexBuffer> m_VBO;
    std::shared_ptr<IndexBuffer> m_EBO;

    std::shared_ptr<Material> m_CurrentMaterial = nullptr;

    uint32_t m_IndicesSize = 0;
    uint32_t m_CurrentSubmitCount = 0;
};

} // namespace Forge

#endif
