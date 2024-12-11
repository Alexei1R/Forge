
#ifndef RENDERERBATCH_H
#define RENDERERBATCH_H


#include "Forge/Renderer/Buffer/Buffer.h"
#include "Forge/Renderer/Buffer/BufferImpl.h"
#include "Forge/Renderer/Material.h"
#include "Forge/Renderer/Mesh.h"
#include "glm/fwd.hpp"
#include <cstdint>
#include <vector>
namespace Forge {

class RendererBatch
{
public:
    RendererBatch() = default;
    ~RendererBatch() = default;

    RendererBatch(const RendererBatch&) = delete;
    RendererBatch& operator=(const RendererBatch&) = delete;

    RendererBatch(RendererBatch&&) noexcept = delete;
    RendererBatch& operator=(RendererBatch&&) noexcept = delete;

    void Init(const uint32_t vertexSize, const uint32_t indexSize, const BufferLayout& layout);
    void Submit(const std::vector<uint8_t>& vertices,
                const std::vector<uint32_t>& indices,
                const std::shared_ptr<Material>& material);
    void Flush();

private:
    void SetupBuffers();

private:
    std::vector<uint8_t> m_VerticesBytes;
    std::vector<uint32_t> m_Indices;


    std::shared_ptr<VertexArrayBuffer> m_VAO;
    std::shared_ptr<VertexBuffer> m_VBO;
    std::shared_ptr<IndexBuffer> m_EBO;

    std::shared_ptr<Material> m_CurrentMaterial = nullptr;


    uint32_t m_IndicesSize;
    uint32_t m_CurrentSubmitCount = 0;
    uint32_t m_VertexStackSizeInBytes = 0;
};


}  // namespace Forge


#endif
