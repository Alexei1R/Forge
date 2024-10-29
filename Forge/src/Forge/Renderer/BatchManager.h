//
// Created by toor on 2024-10-29.
//

#ifndef BATCHMANAGER_H
#define BATCHMANAGER_H

#include <vector>
#include <memory>
#include <cstddef>
#include "Forge/Core/Log/Log.h"
#include "Forge/Renderer/Buffer/BufferImpl.h"
#include "Forge/Renderer/Shader.h"

namespace Forge {

class BatchManager
{
public:
    BatchManager(unsigned int capacity);
    void BeginBatch();
    void EndBatch();
    void Flush();

    template <typename VertexType>
    void Submit(const std::vector<VertexType>& vertices, const std::vector<unsigned int>& indices)
    {
        size_t vertexSize = sizeof(VertexType);
        m_VertexBufferSize += vertexSize;

        if ((m_VertexBufferSize / vertexSize) >= m_Capacity)
        {
            LOG_WARN("Please set bigger batch size , Current capacity : {}", m_Capacity)
            m_Capacity *= 2;
            m_ResizeVertexBuffer = true;
        }
        if (m_ResizeVertexBuffer)
        {
            m_ResizeVertexBuffer = false;
            m_VertexBuffer.reserve(m_Capacity * vertexSize);
            m_Indices.reserve(m_Capacity * sizeof(unsigned int));
        }

        const std::byte* data = reinterpret_cast<const std::byte*>(vertices.data());
        m_VertexBuffer.insert(m_VertexBuffer.end(), data, data + vertices.size() * vertexSize);

        m_Indices.insert(m_Indices.end(), indices.begin(), indices.end());
    }


private:
    std::vector<std::byte> m_VertexBuffer = {};
    std::vector<unsigned int> m_Indices = {};
    unsigned int m_VertexBufferSize = 0;
    bool m_ResizeVertexBuffer = true;
    unsigned int m_Capacity;
};

}  // namespace Forge

#endif  // BATCHMANAGER_H
