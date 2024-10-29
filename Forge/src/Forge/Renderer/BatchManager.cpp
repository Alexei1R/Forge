//
// Created by toor on 2024-10-29.
//

#include "BatchManager.h"

namespace Forge {

BatchManager::BatchManager(unsigned int capacity) : m_Capacity(capacity) {}

void BatchManager::BeginBatch()
{
    m_VertexBuffer.clear();
    m_Indices.clear();
    m_VertexBufferSize = 0;
}

void BatchManager::EndBatch() {}

void BatchManager::Flush()
{
    /*LOG_INFO(" VertexBuffer Size Byte : {}", m_VertexBuffer.size());*/
    /*LOG_INFO(" IndexBuffer Size in elements : {}", m_Indices.size());*/
    /**/
    /*LOG_INFO(" Currect Count {0}  ; m_Capacity {1}", (m_VertexBufferSize / 24), m_Capacity);*/
}


}  // namespace Forge
