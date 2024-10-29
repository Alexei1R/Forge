//
// Created by toor on 2024-10-29.
//

#include "BatchManager.h"
#include "Forge/Core/Log/Log.h"
#include <stdexcept>

namespace Forge {

BatchManager::BatchManager(unsigned int capacity) :
    m_Capacity(capacity), m_VertexBufferSize(0), m_VertexTypeSize(0)
{
}

void BatchManager::BeginBatch() {}

void BatchManager::EndBatch() {}

void BatchManager::Flush()
{
    LOG_INFO(" VetexType : {}", m_VertexTypeSize)
    LOG_INFO(" Size : {}", m_VertexBufferSize)
    LOG_INFO(" Size Byte : {}", m_VertexBuffer.size())
    LOG_INFO(" Count : {}", m_VertexBufferSize / m_VertexTypeSize)
}

}  // namespace Forge
