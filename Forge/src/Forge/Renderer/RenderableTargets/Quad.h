#ifndef QUADVERTEX_H
#define QUADVERTEX_H

#include "Forge/Renderer/RenderableTarget.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
namespace Forge {

struct QuadVertex
{
    glm::vec3 Position;
    glm::vec4 Color;
};

}  // namespace Forge
#endif

// Primitives/Quad.h
#ifndef QUAD_H
#define QUAD_H

namespace Forge {

class Quad : public RenderableTarget<QuadVertex>
{
public:
    Quad()
    {
        m_Layout = {{BufferDataType::Float3, "a_Position"}, {BufferDataType::Float4, "a_Color"}};
    }

    std::vector<QuadVertex> GetVertices(const glm::vec3& position,
                                        const glm::vec2& size,
                                        const glm::vec4& color) const override
    {
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
                              glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});


        return {{transform * glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f), color},
                {transform * glm::vec4(0.5f, -0.5f, 0.0f, 1.0f), color},
                {transform * glm::vec4(0.5f, 0.5f, 0.0f, 1.0f), color},
                {transform * glm::vec4(-0.5f, 0.5f, 0.0f, 1.0f), color}};
    }

    std::vector<unsigned int> GetIndices() const override
    {
        return {0, 1, 2, 2, 3, 0};
    }

    const BufferLayout& GetLayout() const override
    {
        return m_Layout;
    }

private:
    BufferLayout m_Layout;
};

}  // namespace Forge
#endif
