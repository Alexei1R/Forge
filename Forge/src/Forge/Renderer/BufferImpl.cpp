//
// Created by toor on 2024-09-23.
//

#include "BufferImpl.h"


namespace Forge {

// Utility functions (if not already implemented elsewhere)
unsigned int GetComponentCount(BufferDataType type)
{
    switch (type)
    {
        case BufferDataType::Float: return 1;
        case BufferDataType::Float2: return 2;
        case BufferDataType::Float3: return 3;
        case BufferDataType::Float4: return 4;
        case BufferDataType::Mat3: return 3 * 3;  // 9 components
        case BufferDataType::Mat4: return 4 * 4;  // 16 components
        case BufferDataType::Int: return 1;
        case BufferDataType::Int2: return 2;
        case BufferDataType::Int3: return 3;
        case BufferDataType::Int4: return 4;
        case BufferDataType::Bool: return 1;
        default: return 0;
    }
}

GLenum BufferDataTypeToOpenGLBaseType(BufferDataType type)
{
    switch (type)
    {
        case BufferDataType::Float:
        case BufferDataType::Float2:
        case BufferDataType::Float3:
        case BufferDataType::Float4:
        case BufferDataType::Mat3:
        case BufferDataType::Mat4: return GL_FLOAT;
        case BufferDataType::Int:
        case BufferDataType::Int2:
        case BufferDataType::Int3:
        case BufferDataType::Int4: return GL_INT;
        case BufferDataType::Bool: return GL_BOOL;
        default: LOG_ERROR("Unknown BufferDataType!"); return 0;
    }
}

//========================================
//  Vertex Buffer Implementation
//========================================

VertexBuffer::VertexBuffer(const void* data, uint32_t size)
{
    glGenBuffers(1, &m_RendererID);
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

    LOG_INFO("VertexBuffer created with ID {0}", m_RendererID);
}

VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &m_RendererID);
    LOG_INFO("VertexBuffer with ID {0} deleted", m_RendererID);
}

void VertexBuffer::Bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
}

void VertexBuffer::Unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// GetLayout() and SetLayout() are defined inline in the header

//========================================
//  Index Buffer Implementation
//========================================

IndexBuffer::IndexBuffer(uint32_t* data, uint32_t count) : m_Count(count)
{
    glGenBuffers(1, &m_RendererID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), data, GL_STATIC_DRAW);

    LOG_INFO("IndexBuffer created with ID {0}", m_RendererID);
}

IndexBuffer::~IndexBuffer()
{
    glDeleteBuffers(1, &m_RendererID);
    LOG_INFO("IndexBuffer with ID {0} deleted", m_RendererID);
}

void IndexBuffer::Bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
}

void IndexBuffer::Unbind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

//========================================
//  Vertex Array Buffer Implementation
//========================================

VertexArrayBuffer::VertexArrayBuffer()
{
    glGenVertexArrays(1, &m_RendererID);
    LOG_INFO("VertexArrayBuffer created with ID {0}", m_RendererID);
}

VertexArrayBuffer::~VertexArrayBuffer()
{
    glDeleteVertexArrays(1, &m_RendererID);
    LOG_INFO("VertexArrayBuffer with ID {0} deleted", m_RendererID);
}

void VertexArrayBuffer::Bind() const
{
    glBindVertexArray(m_RendererID);
}

void VertexArrayBuffer::Unbind() const
{
    glBindVertexArray(0);
}

void VertexArrayBuffer::AddVertexBuffer(std::shared_ptr<VertexBuffer>& vertexBuffer)
{
    // Ensure the vertex buffer has a layout
    if (vertexBuffer->GetLayout().GetElements().empty())
    {
        LOG_ERROR("VertexBuffer has no layout!");
        return;
    }

    glBindVertexArray(m_RendererID);
    vertexBuffer->Bind();

    const auto& layout = vertexBuffer->GetLayout();
    uint32_t index = 0;
    for (const auto& element : layout)
    {
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(
            index,
            GetComponentCount(element.type),
            BufferDataTypeToOpenGLBaseType(element.type),
            GL_FALSE,
            layout.GetStride(),
            (const void*)(intptr_t)element.offset);
        index++;
    }

    m_VertexBuffers.push_back(vertexBuffer);
    LOG_INFO("VertexBuffer added to VertexArrayBuffer {0}", m_RendererID);
}

void VertexArrayBuffer::SetIndexBuffer(std::shared_ptr<IndexBuffer>& indexBuffer)
{
    glBindVertexArray(m_RendererID);
    indexBuffer->Bind();

    m_IndexBuffer = indexBuffer;
    LOG_INFO("IndexBuffer set for VertexArrayBuffer {0}", m_RendererID);
}

}  // namespace Forge
