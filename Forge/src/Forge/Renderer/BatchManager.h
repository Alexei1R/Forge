//
// Created by toor on 2024-10-29.
//

#ifndef BATCHMANAGER_H
#define BATCHMANAGER_H

#include <cstdint>
#include <vector>
#include "Forge/Core/Log/Log.h"
#include "Forge/Renderer/Buffer/Buffer.h"
#include "Forge/Renderer/Buffer/BufferImpl.h"
#include "Forge/Renderer/Material.h"
#include "Forge/Renderer/RenderCommand.h"
#include "Forge/Renderer/ShaderManager.h"
#include "Forge/Renderer/TextureManager.h"

namespace Forge {

struct BatchProps
{
    uint32_t Capacity;
    uint32_t VerticesCount = 32;
    uint32_t IndicesCount = 32;
    const BufferLayout& bufferLayout;

    BatchProps(unsigned int capacity,
               uint32_t verticesCount,
               uint32_t indicesCount,
               const BufferLayout& bufferLayout) :
        Capacity(capacity), VerticesCount(verticesCount), IndicesCount(indicesCount),
        bufferLayout(bufferLayout)
    {
    }
};

template <typename VertexType>
class BatchManager
{
public:
    BatchManager(const BatchProps& properties) : m_BatchProperties(properties)
    {
        // Initialize buffers with appropriate capacity
        uint32_t MaxVertexCount = m_BatchProperties.Capacity * m_BatchProperties.VerticesCount;
        uint32_t MaxIndexCount = m_BatchProperties.Capacity * m_BatchProperties.IndicesCount;

        m_Vertices.reserve(MaxVertexCount);
        m_Indices.reserve(MaxIndexCount);

        m_VAO = std::make_shared<VertexArrayBuffer>();
        m_VBO = std::make_shared<VertexBuffer>(nullptr,
                                               MaxVertexCount * sizeof(VertexType),
                                               BufferDrawMode::Dynamic);
        m_EBO = std::make_shared<IndexBuffer>(nullptr,
                                              MaxIndexCount * sizeof(uint32_t),
                                              BufferDrawMode::Dynamic);

        m_VAO->Bind();
        m_VBO->Bind();
        m_VBO->SetLayout(m_BatchProperties.bufferLayout);
        m_VAO->AddVertexBuffer(m_VBO);
        m_EBO->Bind();
        m_VAO->SetIndexBuffer(m_EBO);
        m_VAO->Unbind();
    }

    void BeginBatch()
    {
        m_DrawCallCount = 0;
    }

    void EndBatch()
    {
        Flush();
        /*LOG_INFO("Nr DrawCalls is {}", m_DrawCallCount);*/
    }


    void Submit(const std::vector<VertexType>& vertices,
                const std::vector<uint32_t>& indices,
                const std::shared_ptr<Material>& material)
    {
        if (m_CurrentMaterial != material)
        {
            Flush();
            m_CurrentMaterial = material;
        }

        // Ensure batch has space
        if (vertices.size() + m_Vertices.size() >
                m_BatchProperties.Capacity * m_BatchProperties.VerticesCount ||
            indices.size() + m_Indices.size() >
                m_BatchProperties.Capacity * m_BatchProperties.IndicesCount)
        {
            Flush();
        }

        m_Vertices.insert(m_Vertices.end(), vertices.begin(), vertices.end());
        uint32_t currentVertexOffset = static_cast<uint32_t>(m_Vertices.size() - vertices.size());

        // Offset indices and add
        for (uint32_t index : indices)
        {
            m_Indices.push_back(index + currentVertexOffset);
        }


        if (material->TextureHandles.size() > 0)
        {
            m_HasTextures = true;
        }

        // if texture count is biiger er than maz texture slot flush
    }

    void Flush()
    {
        if (m_Vertices.empty())
            return;

        m_VAO->Bind();
        auto& shaderManager = ShaderManager::GetInstance();
        auto shader = shaderManager.GetShader(m_CurrentMaterial->ShaderHandle);
        shader->Bind();
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

        m_VBO->Bind();
        m_VBO->SubmitData(m_Vertices.data(), m_Vertices.size() * sizeof(VertexType));
        m_EBO->Bind();
        m_EBO->SubmitData(m_Indices.data(), m_Indices.size() * sizeof(uint32_t));

        RenderCommand::DrawIndexed(m_VAO, m_Indices.size());
        m_DrawCallCount++;

        m_VAO->Unbind();

        // Clear vectors without deallocation
        m_Vertices.clear();
        m_Indices.clear();
    }

private:
    const BatchProps m_BatchProperties;
    std::shared_ptr<VertexArrayBuffer> m_VAO;
    std::shared_ptr<VertexBuffer> m_VBO;
    std::shared_ptr<IndexBuffer> m_EBO;

    std::vector<VertexType> m_Vertices;
    std::vector<uint32_t> m_Indices;

    uint32_t m_DrawCallCount = 0;
    std::shared_ptr<Material> m_CurrentMaterial = nullptr;

    bool m_HasTextures = false;
};

}  // namespace Forge

#endif  // BATCHMANAGER_H
