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
#include "Forge/Renderer/RenderCommand.h"

namespace Forge {


struct BatchProps
{
    uint32_t Capacity;
    uint32_t IndicesCount = 32;
    const BufferLayout& bufferLayout;

    bool HasTextures = false;
    BatchProps(unsigned int capacity,
               uint32_t indicesCount,
               const BufferLayout& bufferLayout,
               bool hasTextures = false) :
        Capacity(capacity), IndicesCount(indicesCount), bufferLayout(bufferLayout),
        HasTextures(hasTextures)
    {
    }
};


template <typename VertexType>
class BatchManager
{
public:
    BatchManager(const BatchProps& properties) : m_BatchProperties(properties)
    {
        m_VertexBuffer.reserve(m_BatchProperties.Capacity);
        m_IndexBufffer.reserve(m_BatchProperties.Capacity * m_BatchProperties.IndicesCount);
        m_TextureSlots = RenderCommand::GetMaxTextureSlots();

        // NOTE: Initialize buffers
        if ((m_BatchProperties.bufferLayout.GetElements().size() <= 0))
        {
            F_ASSERT(false, "BatchManager BufferLayout is not set");
        }
    }
    void BeginBatch()
    {
        m_VertexBuffer.clear();
        m_IndexBufffer.clear();
    }
    void EndBatch()
    {
        Flush();
    }

    void Submit(const std::vector<VertexType>& vertices, const std::vector<uint32_t>& indices)
    {
        if (vertices.size() + m_VertexBuffer.size() > m_BatchProperties.Capacity ||
            indices.size() + m_IndexBufffer.size() >
                m_BatchProperties.Capacity * m_BatchProperties.IndicesCount)
        {
            Flush();
        }

        m_VertexBuffer.insert(m_VertexBuffer.end(), vertices.begin(), vertices.end());
        m_VertexCount++;

        for (auto index : indices)
        {
            m_IndexBufffer.push_back(
                index + static_cast<uint32_t>(m_VertexBuffer.size() - vertices.size()));
        }
    }

    void Flush()
    {
        if (m_IndexBufffer.empty())
        {
            return;  // Nothing to draw
        }

        m_VAO->Bind();

        m_VBO->SetLayout(m_BatchProperties.bufferLayout);
        m_VAO->AddVertexBuffer(m_VBO);

        m_EBO->Bind();
        m_VAO->SetIndexBuffer(m_EBO);
        // Upload vertex data
        m_VBO->Bind();
        m_VBO->SubmitData(m_VertexBuffer.data(), m_VertexBuffer.size() * sizeof(VertexType));

        m_EBO->Bind();
        m_EBO->SubmitData(m_IndexBufffer.data(), m_IndexBufffer.size() * sizeof(uint32_t));


        // Bind textures if any
        if (m_BatchProperties.HasTextures)
        {
            LOG_CRITICAL("Can`t batch textures")
        }

        LOG_CRITICAL("Draw VAO")
        // Draw call
        RenderCommand::Draw(m_VAO);

        m_VAO->Unbind();
    }


private:
    std::vector<VertexType> m_VertexBuffer = {};
    std::vector<uint32_t> m_IndexBufffer = {};


    uint32_t m_VertexCount = 0;
    uint8_t m_TextureSlots = 0;
    const BatchProps m_BatchProperties;


    std::shared_ptr<VertexArrayBuffer> m_VAO;
    std::shared_ptr<VertexBuffer> m_VBO;
    std::shared_ptr<IndexBuffer> m_EBO;
};

}  // namespace Forge

#endif  // BATCHMANAGER_H
