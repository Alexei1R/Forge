#ifndef BATCHMANAGER_H
#define BATCHMANAGER_H

#include <array>
#include <cstdint>
#include <vector>

#include "Forge/Core/Log/Log.h"
#include "Forge/Renderer/Buffer/Buffer.h"
#include "Forge/Renderer/Buffer/BufferImpl.h"
#include "Forge/Renderer/RenderCommand.h"
#include "Forge/Renderer/Texture.h" // Include Texture header if handling textures

namespace Forge {

struct BatchProps {
    uint32_t Capacity;
    uint32_t IndicesCount = 32;
    const BufferLayout& bufferLayout;
    bool HasTextures = false;

    BatchProps(uint32_t capacity, uint32_t indicesCount, const BufferLayout& bufferLayout, bool hasTextures = false)
        : Capacity(capacity)
        , IndicesCount(indicesCount)
        , bufferLayout(bufferLayout)
        , HasTextures(hasTextures) {}
};

template <typename VertexType>
class BatchManager {
public:
    BatchManager(const BatchProps& properties)
        : m_BatchProperties(properties)
        , m_TextureSlotIndex(0) {
        m_VertexBuffer.reserve(m_BatchProperties.Capacity);
        m_IndexBufffer.reserve(m_BatchProperties.Capacity * m_BatchProperties.IndicesCount);
        m_TextureSlots = RenderCommand::GetMaxTextureSlots();

        // Initialize buffers
        if (m_BatchProperties.bufferLayout.GetElements().empty()) {
            F_ASSERT(false, "BatchManager BufferLayout is not set");
        }

        m_VAO = std::make_shared<VertexArrayBuffer>();
        m_VBO = std::make_shared<VertexBuffer>(nullptr, m_BatchProperties.Capacity * sizeof(VertexType), BufferDrawMode::Dynamic);

        m_EBO = std::make_shared<IndexBuffer>(nullptr, m_BatchProperties.Capacity * m_BatchProperties.IndicesCount * sizeof(uint32_t),
                                              BufferDrawMode::Dynamic);

        m_VAO->Bind();
        m_VBO->Bind();

        m_VBO->SetLayout(m_BatchProperties.bufferLayout);
        m_VAO->AddVertexBuffer(m_VBO);

        m_EBO->Bind();
        m_VAO->SetIndexBuffer(m_EBO);

        m_VAO->Unbind();

        // Initialize texture slots if using textures
        if (m_BatchProperties.HasTextures) {
            m_TextureSlotArray.fill(nullptr);
        }
    }

    void BeginBatch() {
        m_VertexBuffer.clear();
        m_IndexBufffer.clear();
        m_TextureSlotIndex = 0;
        if (m_BatchProperties.HasTextures) {
            m_TextureSlotArray.fill(nullptr);
        }
    }

    void EndBatch() {
        Flush();
    }

    void Flush() {
        if (m_IndexBufffer.empty())
            return; // Nothing to draw

        m_VAO->Bind();

        // Upload vertex data
        m_VBO->Bind();
        m_VBO->SubmitData(m_VertexBuffer.data(), m_VertexBuffer.size() * sizeof(VertexType));

        // Upload index data
        m_EBO->Bind();
        m_EBO->SubmitData(m_IndexBufffer.data(), m_IndexBufffer.size() * sizeof(uint32_t));

        // Bind textures if any
        if (m_BatchProperties.HasTextures) {
            for (uint32_t i = 0; i < m_TextureSlotIndex; ++i) {
                m_TextureSlotArray[i]->Bind(i);
            }
        }

        // Draw call
        RenderCommand::Draw(m_VAO);

        m_VAO->Unbind();
    }

    void Submit(const std::vector<VertexType>& vertices, const std::vector<uint32_t>& indices,
                const std::shared_ptr<Texture>& texture = nullptr) {
        // Check if we need to flush
        if (vertices.size() + m_VertexBuffer.size() > m_BatchProperties.Capacity ||
            indices.size() + m_IndexBufffer.size() > m_BatchProperties.Capacity * m_BatchProperties.IndicesCount ||
            (texture != nullptr && m_TextureSlotIndex >= m_TextureSlots)) {
            Flush();
            BeginBatch(); // Reset batch data after flushing
        }

        uint32_t textureIndex = 0;
        if (texture != nullptr) {
            // Check if texture is already in slot array
            bool found = false;
            for (uint32_t i = 0; i < m_TextureSlotIndex; ++i) {
                if (m_TextureSlotArray[i]->GetID() == texture->GetID()) {
                    textureIndex = i;
                    found = true;
                    break;
                }
            }
            if (!found) {
                // Add new texture
                textureIndex = m_TextureSlotIndex;
                m_TextureSlotArray[m_TextureSlotIndex] = texture;
                m_TextureSlotIndex++;
            }
        }

        // Adjust vertices to set the texture index
        std::vector<VertexType> adjustedVertices = vertices;
        if (texture != nullptr) {
            for (auto& vertex : adjustedVertices) {
                vertex.TextureIndex = static_cast<float>(textureIndex);
            }
        }

        // Append adjusted vertices to m_VertexBuffer
        m_VertexBuffer.insert(m_VertexBuffer.end(), adjustedVertices.begin(), adjustedVertices.end());

        // Adjust indices
        uint32_t indexOffset = static_cast<uint32_t>(m_VertexBuffer.size() - adjustedVertices.size());
        for (auto index : indices) {
            m_IndexBufffer.push_back(index + indexOffset);
        }
    }

private:
    std::vector<VertexType> m_VertexBuffer;
    std::vector<uint32_t> m_IndexBufffer;

    uint8_t m_TextureSlots = 0;
    uint32_t m_TextureSlotIndex = 0;
    std::array<std::shared_ptr<Texture>, 32> m_TextureSlotArray;

    const BatchProps m_BatchProperties;

    std::shared_ptr<VertexArrayBuffer> m_VAO;
    std::shared_ptr<VertexBuffer> m_VBO;
    std::shared_ptr<IndexBuffer> m_EBO;
};

} // namespace Forge

#endif // BATCHMANAGER_H
