//
// Created by toor on 2024-10-29.
//

#ifndef QUAD_H
#define QUAD_H

#include "Forge/Renderer/RenderableTarget.h"
#include "Forge/Renderer/Shader.h"
#include "glm/fwd.hpp"
#include <memory>
#include <vector>

namespace Forge {

struct QuadVertex
{
    glm::vec3 pos;
    glm::vec2 texCoord;
    float texIndex;
};

class Quad : public RenderableTarget<QuadVertex>
{
public:
    Quad()
    {
        vertices = {{glm::vec3 {-0.5f, -0.5f, 0.0f}, glm::vec2 {0.0f, 0.0f}, 0.0},
                    {glm::vec3 {0.5f, -0.5f, 0.0f}, glm::vec2 {1.0f, 0.0f}, 0.0},
                    {glm::vec3 {0.5f, 0.5f, 0.0f}, glm::vec2 {1.0f, 1.0f}, 0.0},
                    {glm::vec3 {-0.5f, 0.5f, 0.0f}, glm::vec2 {0.0f, 1.0f}, 0.0}};

        indices = {0, 1, 2, 2, 3, 0};


        m_Shader = std::make_shared<Shader>(
            ShaderLayout {{ShaderType::VERTEX, "Assets/Shaders/Renderer2DQuad.vert"},
                          {ShaderType::FRAGMENT, "Assets/Shaders/Renderer2DQuad.frag"}});
        m_Shader->Build();


        m_Layout = {{BufferDataType::Float3, "aPos"},
                    {BufferDataType::Float3, "aNormal"},
                    {BufferDataType::Float, "aTexCoords"}};
    }

    const BufferLayout& GetBufferLayout() override
    {
        return m_Layout;
    }


    const std::vector<QuadVertex>& GetVertices() const override
    {
        return vertices;
    }

    const std::vector<unsigned int>& GetIndices(int offset = 0) const override
    {
        return indices;
    }

    const std::shared_ptr<Shader>& GetShader() override
    {
        return m_Shader;
    }


private:
    std::vector<QuadVertex> vertices = {};
    std::vector<unsigned int> indices = {};
    std::shared_ptr<Shader> m_Shader;

    BufferLayout m_Layout;
};

}  // namespace Forge

#endif  // QUAD_H
