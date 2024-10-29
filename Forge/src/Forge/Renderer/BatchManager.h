//
// Created by toor on 2024-10-29.
//

#ifndef BATCHMANAGER_H
#define BATCHMANAGER_H

#include <algorithm>
#include <vector>
#include <cstddef>
#include "Forge/Core/Log/Log.h"


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
        m_VertexTypeSize = vertexSize;
        m_VertexBufferSize += vertexSize;

        if ((m_VertexBufferSize / m_VertexTypeSize) >= m_Capacity)
        {
            LOG_WARN("Please set bigger batch size")
            m_Capacity *= 2;
            m_IsResizedToCapacity = false;
        }
        if (!m_IsResizedToCapacity)
        {
            m_IsResizedToCapacity = true;
            LOG_INFO("Resize VertexBuffer")
            m_VertexBuffer.reserve(m_Capacity * vertices.size());
            m_Indices.reserve(m_Capacity * indices.size());
        }

        const std::byte* data = reinterpret_cast<const std::byte*>(vertices.data());
        m_VertexBuffer.insert(m_VertexBuffer.end(), data, data + vertices.size() * vertexSize);
    }

private:
    std::vector<std::byte> m_VertexBuffer;
    unsigned int m_VertexBufferSize;

    unsigned int m_VertexTypeSize;
    std::vector<unsigned int> m_Indices;
    unsigned int m_Capacity;
    bool m_IsResizedToCapacity = false;
};

}  // namespace Forge

#endif
