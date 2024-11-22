//
// Created by toor on 2024-11-20.
//

#ifndef QUAD_H
#define QUAD_H

#include "Forge/Renderer/RenderableTarget.h"
#include "Forge/Renderer/Buffer/Buffer.h"
#include "glm/fwd.hpp"
#include <cstdint>
#include <glm/glm.hpp>
#include <vector>
#include "glm/ext/matrix_transform.hpp"


namespace Forge {

struct QuadVertex
{
    glm::vec3 Position;
    glm::vec4 Color;
};

class Quad : public RenderableTarget
{
public:
    Quad(glm::vec3 position, glm::vec2 size, glm::vec4 color)
    {
        m_Layout = {{BufferDataType::Float3, "a_Position"}, {BufferDataType::Float4, "a_Color"}};
        m_Indices = {0, 1, 2, 2, 3, 0};

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
                              glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

        // Make it clear this is a temporary vector
        std::vector<QuadVertex> vertices = {
            {transform * glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f), color},
            {transform * glm::vec4(0.5f, -0.5f, 0.0f, 1.0f), color},
            {transform * glm::vec4(0.5f, 0.5f, 0.0f, 1.0f), color},
            {transform * glm::vec4(-0.5f, 0.5f, 0.0f, 1.0f), color}};

        m_VerticesBytes.resize(vertices.size() * sizeof(QuadVertex));
        memcpy(m_VerticesBytes.data(), vertices.data(), m_VerticesBytes.size());
    }

    const std::vector<uint8_t>& GetVertices() const override
    {
        return m_VerticesBytes;
    }

    const std::vector<uint32_t>& GetIndices() const override
    {
        return m_Indices;
    }

    const BufferLayout& GetLayout() const override
    {
        return m_Layout;
    }


private:
    BufferLayout m_Layout;
    std::vector<uint32_t> m_Indices;
    std::vector<uint8_t> m_VerticesBytes;
};

}  // namespace Forge
#endif
