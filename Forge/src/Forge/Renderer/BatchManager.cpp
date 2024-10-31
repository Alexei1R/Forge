//
// Created by toor on 2024-10-29.
//

#include "BatchManager.h"
#include "Forge/Core/Log/Log.h"
#include "Forge/Renderer/Buffer/BufferImpl.h"
#include "Forge/Renderer/RenderCommand.h"
#include "Forge/Renderer/Buffer/Buffer.h"
#include <algorithm>
#include <initializer_list>

namespace Forge {

BatchManager::BatchManager(const BatchProps& properties) :
    m_Capacity(properties.Capacity), m_BufferLayout(properties.bufferLayout)
{
}


void BatchManager::BeginBatch()
{
    m_VertexBuffer.clear();
    m_Indices.clear();
    m_VertexBufferSize = 0;
    m_VertexOffset = 0;
}

void BatchManager::EndBatch()
{
    if (m_RegenerateBuffers)
    {
        m_RegenerateBuffers = false;
        if ((m_BufferLayout.GetElements().size() <= 0))
        {
            F_ASSERT(false, "BatchManager Shader or bufferLayout is not set");
        }

        m_VAO.reset();
        m_VBO.reset();
        m_EBO.reset();


        m_VAO = std::make_shared<VertexArrayBuffer>();
        m_VBO = std::make_shared<VertexBuffer>(nullptr,
                                               m_VertexBuffer.size(),
                                               VertexBufferDrawMode::Dynamic);


        m_EBO = std::make_shared<IndexBuffer>(m_Indices.data(),
                                              m_Indices.size() * sizeof(unsigned int));

        m_VAO->Bind();
        m_VBO->Bind();


        m_VBO->SetLayout(m_BufferLayout);
        m_VAO->AddVertexBuffer(m_VBO);

        m_EBO->Bind();
        m_VAO->SetIndexBuffer(m_EBO);


        m_VAO->Unbind();
    }
}

void BatchManager::Flush()
{
    if (m_VAO)
    {
        m_VAO->Bind();
        m_VBO->Bind();
        m_VBO->SubmitData(m_VertexBuffer.data(), m_VertexBuffer.size());

        RenderCommand::Draw(m_VAO);
        m_VAO->Unbind();
    }
}


}  // namespace Forge
