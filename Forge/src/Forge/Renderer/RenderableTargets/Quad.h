//
// Created by toor on 2024-10-31.
//


#include "Forge/Renderer/RenderableTarget.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include <cstdint>

#ifndef QUAD_H
#    define QUAD_H

namespace Forge {

struct QuadVertex
{
    glm::vec3 Position;
    glm::vec4 Color;
};

class Quad : public RenderableTarget<QuadVertex>
{
public:
    Quad()
    {
        m_Layout = {{BufferDataType::Float3, "a_Position"}, {BufferDataType::Float4, "a_Color"}};
        m_Indices = {0, 1, 2, 2, 3, 0};
    }

    std::vector<QuadVertex>& GetVertices(const glm::vec3& position,
                                         const glm::vec2& size,
                                         const glm::vec4& color) override
    {
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
                              glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});


        m_Vertices = {{transform * glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f), color},
                      {transform * glm::vec4(0.5f, -0.5f, 0.0f, 1.0f), color},
                      {transform * glm::vec4(0.5f, 0.5f, 0.0f, 1.0f), color},
                      {transform * glm::vec4(-0.5f, 0.5f, 0.0f, 1.0f), color}};

        return m_Vertices;
    }

    const std::vector<uint32_t>& GetIndices() const override
    {
        return m_Indices;
    }

    const BufferLayout& GetLayout() const override
    {
        return m_Layout;
    }

    const uint32_t GetVerticesSize() const
    {
        return 4;
    }
    const uint32_t GetIndicesSize() const
    {
        return 5;
    }


private:
    BufferLayout m_Layout;
    std::vector<uint32_t> m_Indices;
    std::vector<QuadVertex> m_Vertices;
};

}  // namespace Forge
#endif
