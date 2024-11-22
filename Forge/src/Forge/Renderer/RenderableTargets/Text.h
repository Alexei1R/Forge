//
// Created by toor on 2024-11-20.
//

#ifndef TEXT_H
#define TEXT_H

#include "Forge/Renderer/Font.h"
#include "Forge/Renderer/Material.h"
#include "Forge/Renderer/RenderableTarget.h"
#include "Forge/Renderer/Buffer/Buffer.h"
#include "glm/fwd.hpp"
#include <cstdint>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "glm/ext/matrix_transform.hpp"


namespace Forge {


struct TextVertex
{
    glm::vec3 Position;
    glm::vec4 Color;
    glm::vec2 TexCoord;
    float TexIndex;
};


class Text : public RenderableTarget
{
public:
    Text(const std::string& text,
         const glm::vec3& position,
         const float scale,
         const std::shared_ptr<Font>& font,
         const std::shared_ptr<Material>& material);

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


    void Update(const std::string& text, const glm::vec3& position, const float scale);

private:
    BufferLayout m_Layout;
    std::vector<uint32_t> m_Indices;
    std::vector<uint8_t> m_VerticesBytes;

    std::string m_Text;
    glm::vec3 m_Position;
    float m_Scale;
    std::shared_ptr<Font> m_Font;
    std::shared_ptr<Material> m_Material;
};

}  // namespace Forge
#endif
