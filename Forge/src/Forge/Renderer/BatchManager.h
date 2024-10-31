//
// Created by toor on 2024-10-29.
//

#ifndef BATCHMANAGER_H
#define BATCHMANAGER_H

#include <iostream>
#include <vector>
#include <memory>
#include <cstddef>
#include "Forge/Core/Log/Log.h"
#include "Forge/Renderer/Buffer/Buffer.h"
#include "Forge/Renderer/Buffer/BufferImpl.h"
#include "Forge/Renderer/Shader.h"

namespace Forge {


struct BatchProps
{
    unsigned int Capacity;
    const BufferLayout& bufferLayout;

    BatchProps(unsigned int capacity, const BufferLayout& bufferLayout) :
        Capacity(capacity), bufferLayout(bufferLayout)
    {
    }
};

class BatchManager
{
public:
    BatchManager(const BatchProps& properties);
    void BeginBatch();
    void EndBatch();
    void Flush();

    template <typename VertexType>
    void Submit(const std::vector<VertexType>& vertices, const std::vector<unsigned int>& indices)
    {
        size_t vertexSize = sizeof(VertexType);
        m_VertexTypeSize = vertexSize;
        m_VertexBufferSize += vertexSize;

        while ((m_VertexBufferSize / vertexSize) >= m_Capacity)
        {
            LOG_WARN("Please set bigger batch size , Current capacity : {}", m_Capacity);
            m_Capacity *= 2;
            m_ResizeVertexBuffer = true;
        }

        if (m_ResizeVertexBuffer)
        {
            m_ResizeVertexBuffer = false;
            m_VertexBuffer.reserve(m_Capacity * vertexSize);
            m_Indices.reserve(m_Capacity * indices.size());
            m_VertexBuffer.clear();
            m_Indices.clear();

            m_RegenerateBuffers = true;
            m_VertexOffset = 0;
        }

        const std::byte* data = reinterpret_cast<const std::byte*>(vertices.data());
        m_VertexBuffer.insert(m_VertexBuffer.end(), data, data + vertices.size() * vertexSize);

        for (unsigned int index : indices)
        {
            m_Indices.push_back(index + m_VertexOffset);
        }

        m_VertexOffset += vertices.size();
        m_IndexCount++;
    }

private:
    std::vector<std::byte> m_VertexBuffer = {};
    std::vector<unsigned int> m_Indices = {};
    unsigned int m_VertexTypeSize = 0;
    unsigned int m_VertexBufferSize = 0;
    bool m_ResizeVertexBuffer = true;
    unsigned int m_Capacity;

    unsigned int m_IndexCount = 0;
    unsigned int m_VertexOffset = 0;

    const BufferLayout& m_BufferLayout;


    bool m_RegenerateBuffers = false;
    std::shared_ptr<VertexArrayBuffer> m_VAO;
    std::shared_ptr<VertexBuffer> m_VBO;
    std::shared_ptr<IndexBuffer> m_EBO;
};

}  // namespace Forge

#endif  // BATCHMANAGER_H
