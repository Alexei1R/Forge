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

    uint32_t VerticesCount = 32;
    uint32_t IndicesCount = 32;
    const BufferLayout& bufferLayout;

    bool HasTextures = false;
    BatchProps(unsigned int capacity,
               uint32_t verticesCount,
               uint32_t indicesCount,
               const BufferLayout& bufferLayout,
               bool hasTextures = false) :
        Capacity(capacity), VerticesCount(verticesCount), IndicesCount(indicesCount),
        bufferLayout(bufferLayout), HasTextures(hasTextures)
    {
    }
};


template <typename VertexType>
class BatchManager
{
public:
    BatchManager(const BatchProps& properties) : m_BatchProperties(properties)
    {
        // NOTE: Initialize buffers
        if ((m_BatchProperties.bufferLayout.GetElements().size() <= 0))
        {
            F_ASSERT(false, "BatchManager BufferLayout is not set");
        }

        uint32_t MaxVertexCount = m_BatchProperties.Capacity * m_BatchProperties.VerticesCount;
        uint32_t MaxIndexCount = m_BatchProperties.Capacity * m_BatchProperties.IndicesCount;

        // Reserve space for vertices and indices
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
    void BeginBatch() {}
    void EndBatch()
    {
        Flush();
    }

    void Submit(const std::vector<VertexType>& vertices, const std::vector<uint32_t>& indices)
    {
        if (vertices.size() + m_Vertices.size() > m_BatchProperties.Capacity ||
            indices.size() + m_Indices.size() >
                m_BatchProperties.Capacity * m_BatchProperties.IndicesCount)
        {
            Flush();
        }

        if (vertices.size() > m_BatchProperties.Capacity * m_BatchProperties.VerticesCount ||
            indices.size() > m_BatchProperties.Capacity * m_BatchProperties.IndicesCount)
        {
            F_ASSERT(false, "Submitted data exceeds batch capacity");
            return;
        }

        m_Vertices.insert(m_Vertices.end(), vertices.begin(), vertices.end());

        // Adjust indices to offset by current vertex count
        uint32_t currentVertexOffset = static_cast<uint32_t>(m_Vertices.size() - vertices.size());
        for (uint32_t index : indices)
        {
            m_Indices.push_back(index + currentVertexOffset);
        }
    }

    void Flush()
    {
        if (m_Vertices.empty())
            return;  // Nothing to draw

        m_VAO->Bind();

        // Upload vertex data
        m_VBO->Bind();
        m_VBO->SubmitData(m_Vertices.data(), m_Vertices.size() * sizeof(VertexType));

        m_EBO->Bind();
        m_EBO->SubmitData(m_Indices.data(), m_Indices.size() * sizeof(uint32_t));

        RenderCommand::DrawIndexed(m_VAO, m_Indices.size());

        m_VAO->Unbind();

        // Clear vertex and index data for next frame
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
};

}  // namespace Forge

#endif  // BATCHMANAGER_H
