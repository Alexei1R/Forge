
#include "BufferImpl.h"

#include <cassert>
#include <cstdint>

#include "Forge/Core/Log/Log.h"
#include "Forge/Core/Utils.h"
#include "Forge/Renderer/Buffer/Buffer.h"

namespace Forge {

uint32_t GetComponentCount(BufferDataType type) {
    switch (type) {
    case BufferDataType::Float:
        return 1;
    case BufferDataType::Float2:
        return 2;
    case BufferDataType::Float3:
        return 3;
    case BufferDataType::Float4:
        return 4;
    case BufferDataType::Mat3:
        return 3 * 3;
    case BufferDataType::Mat4:
        return 4 * 4;
    case BufferDataType::Int:
        return 1;
    case BufferDataType::Int2:
        return 2;
    case BufferDataType::Int3:
        return 3;
    case BufferDataType::Int4:
        return 4;
    case BufferDataType::Bool:
        return 1;
    default:
        return 0;
    }
}

GLenum BufferDataTypeToOpenGLBaseType(BufferDataType type) {
    switch (type) {
    case BufferDataType::Float:
    case BufferDataType::Float2:
    case BufferDataType::Float3:
    case BufferDataType::Float4:
    case BufferDataType::Mat3:
    case BufferDataType::Mat4:
        return GL_FLOAT;
    case BufferDataType::Int:
    case BufferDataType::Int2:
    case BufferDataType::Int3:
    case BufferDataType::Int4:
        return GL_INT;
    case BufferDataType::Bool:
        return GL_BOOL;
    default:
        LOG_ERROR("Unknown BufferDataType!");
        return 0;
    }
}

//========================================
//  Vertex Buffer Implementation
//========================================

VertexBuffer::VertexBuffer(const void* data, uint32_t count, BufferDrawMode drawMode)
    : m_DrawMode(drawMode) {
    glGenBuffers(1, &m_RendererID);
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);

    switch (drawMode) {
    case BufferDrawMode::Static:
        glBufferData(GL_ARRAY_BUFFER, count, data, GL_STATIC_DRAW);
        break;
    case BufferDrawMode::Dynamic:
        glBufferData(GL_ARRAY_BUFFER, count * sizeof(float), data, GL_DYNAMIC_DRAW);
        break;
    }
}

void VertexBuffer::SubmitData(const void* data, uint32_t count, uint32_t offset) {
    Bind();

    if (m_DrawMode == BufferDrawMode::Dynamic) {
        glBufferSubData(GL_ARRAY_BUFFER, offset, count, data);
    } else {
        F_ASSERT(false, "Can't submit data to a static VertexBuffer. Set BufferDrawMode to "
                        "Dynamic.");
    }
}

VertexBuffer::~VertexBuffer() {
    glDeleteBuffers(1, &m_RendererID);
}

void VertexBuffer::Bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
}

void VertexBuffer::Unbind() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// GetLayout() and SetLayout() are defined inline in the header

//========================================
//  Index Buffer Implementation
//========================================

IndexBuffer::IndexBuffer(uint32_t* data, uint32_t count, BufferDrawMode drawMode)
    : m_Count(count)
    , m_DrawMode(drawMode) {
    glGenBuffers(1, &m_RendererID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);

    switch (drawMode) {
    case BufferDrawMode::Static:
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count, data, GL_STATIC_DRAW);
        break;
    case BufferDrawMode::Dynamic:
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count, data, GL_STATIC_DRAW);
        break;
    }
}

void IndexBuffer::SubmitData(const void* data, uint32_t count, uint32_t offset) {
    Bind();

    if (m_DrawMode == BufferDrawMode::Dynamic) {
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, count, data);
    } else {
        F_ASSERT(false, "Can't submit data to a static IndexBuffer. Set BufferDrawMode to "
                        "Dynamic.");
    }
}

IndexBuffer::~IndexBuffer() {
    glDeleteBuffers(1, &m_RendererID);
}

void IndexBuffer::Bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
}

void IndexBuffer::Unbind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

//========================================
//  Vertex Array Buffer Implementation
//========================================

VertexArrayBuffer::VertexArrayBuffer() {
    glGenVertexArrays(1, &m_RendererID);
}

VertexArrayBuffer::~VertexArrayBuffer() {
    glDeleteVertexArrays(1, &m_RendererID);
}

void VertexArrayBuffer::Bind() const {
    glBindVertexArray(m_RendererID);
}

void VertexArrayBuffer::Unbind() const {
    glBindVertexArray(0);
}

void VertexArrayBuffer::AddVertexBuffer(std::shared_ptr<VertexBuffer>& vertexBuffer) {
    if (vertexBuffer->GetLayout().GetElements().empty()) {
        LOG_ERROR("VertexBuffer has no layout!");
        return;
    }

    glBindVertexArray(m_RendererID);
    vertexBuffer->Bind();

    const auto& layout = vertexBuffer->GetLayout();
    uint32_t index = 0;
    for (const auto& element : layout) {
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index, GetComponentCount(element.type), BufferDataTypeToOpenGLBaseType(element.type), GL_FALSE,
                              layout.GetStride(), (const void*)(intptr_t)element.offset);
        index++;
    }

    m_VertexBuffers.push_back(vertexBuffer);
}

void VertexArrayBuffer::SetIndexBuffer(std::shared_ptr<IndexBuffer>& indexBuffer) {
    glBindVertexArray(m_RendererID);
    indexBuffer->Bind();

    m_IndexBuffer = indexBuffer;
}

} // namespace Forge
