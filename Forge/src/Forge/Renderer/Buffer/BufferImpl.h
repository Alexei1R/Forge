
#ifndef BUFFERIMPL_H
#define BUFFERIMPL_H

#include <glad/glad.h>

#include <cstdint>
#include <glm/glm.hpp>
#include <vector>

#include "Buffer.h"

namespace Forge {

//========================================
//  Vertex Buffer
//========================================

enum class BufferDrawMode : uint8_t { Static, Dynamic };

class VertexBuffer : public Buffer {
public:
    VertexBuffer(const void* data, uint32_t count, BufferDrawMode drawMode = BufferDrawMode::Static);
    ~VertexBuffer();

    void Bind() const override;
    void Unbind() const override;
    void SubmitData(const void* data, uint32_t count, uint32_t offset = 0);

    const BufferLayout& GetLayout() {
        return m_Layout;
    }
    void SetLayout(const BufferLayout& layout) {
        m_Layout = layout;
    }

private:
    uint32_t m_RendererID;
    BufferLayout m_Layout;

    BufferDrawMode m_DrawMode;
};

//========================================
//  Index Buffer
//========================================

class IndexBuffer : public Buffer {
public:
    IndexBuffer(uint32_t* data, uint32_t count, BufferDrawMode drawMode = BufferDrawMode::Static);
    ~IndexBuffer();

    IndexBuffer(const IndexBuffer&) = delete;
    IndexBuffer& operator=(const IndexBuffer&) = delete;

    void Bind() const override;
    void Unbind() const override;
    void SubmitData(const void* data, uint32_t count, uint32_t offset = 0);

    inline uint32_t GetCount() const {
        return m_Count;
    }

private:
    uint32_t m_RendererID;
    uint32_t m_Count;

    BufferDrawMode m_DrawMode;
};

//========================================
//  Vertex Array Buffer
//========================================

class VertexArrayBuffer : public Buffer {
public:
    VertexArrayBuffer();
    ~VertexArrayBuffer();

    void Bind() const override;
    void Unbind() const override;

    void AddVertexBuffer(std::shared_ptr<VertexBuffer>& vertexBuffer);
    void SetIndexBuffer(std::shared_ptr<IndexBuffer>& indexBuffer);

    const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const {
        return m_IndexBuffer;
    }

private:
    std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
    std::shared_ptr<IndexBuffer> m_IndexBuffer;
    uint32_t m_RendererID;
};

} // namespace Forge

#endif
