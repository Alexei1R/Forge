//
// Created by toor on 2024-11-20.
//


#include "Text.h"
#include "Forge/Core/Log/Log.h"
#include <cstdint>


namespace Forge {


Text::Text(const std::string& text,
           const glm::vec3& position,
           const float scale,
           const std::shared_ptr<Font>& font,
           const std::shared_ptr<Material>& material) :
    m_Text(text), m_Position(position), m_Font(font), m_Scale(scale), m_Material(material)
{
    m_Layout = {{BufferDataType::Float3, "a_Position"},
                {BufferDataType::Float4, "a_Color"},
                {BufferDataType::Float2, "a_TexCoord"},
                {BufferDataType::Float, "a_TexIndex"}};
}

void Text::Update(const std::string& text, const glm::vec3& position, const float scale)
{
    const float epsilon = 0.001f;  // Example epsilon value

    if (m_Text == text && m_Position == position && m_Scale == scale)
    {
        return;
    }
    const MSDFData* msdfData = m_Font->GetMSDFData();
    const auto& fontGeometry = msdfData->FontGeometry;
    const auto& metrics = fontGeometry.getMetrics();

    Handle atlasTextureHandle = m_Font->GetAtlasTextureHandle();
    float textureIndex = m_Material->GetTexureSlot(atlasTextureHandle);

    double x = 0.0;
    double y = 0.0;
    double fsScale = scale / (metrics.ascenderY - metrics.descenderY);
    double lineHeight = fsScale * metrics.lineHeight;

    const std::map<std::pair<int, int>, double>& kerningMap = fontGeometry.getKerning();


    std::vector<TextVertex> vertices;
    vertices.reserve(4 * text.size());
    std::vector<uint32_t> indices;
    indices.reserve(6 * text.size());

    // Track global index offset
    uint32_t globalIndexOffset = 0;

    for (size_t i = 0; i < text.size(); ++i)
    {
        char character = text[i];

        // Handle special characters
        if (character == '\r')
            continue;

        if (character == '\n')
        {
            x = 0;
            y -= lineHeight;
            continue;
        }

        if (character == ' ' || character == '\t')
        {
            const msdf_atlas::GlyphGeometry* glyph = nullptr;
            for (const auto& g : msdfData->Glyphs)
            {
                if (g.getCodepoint() == ' ')
                {
                    glyph = &g;
                    break;
                }
            }
            if (!glyph)
                continue;
            double advance = glyph->getAdvance();
            if (character == '\t')
                advance *= 4;  // Handle tab as 4 spaces
            x += fsScale * advance;
            continue;
        }

        // Find the glyph for the character
        const msdf_atlas::GlyphGeometry* glyph = nullptr;
        for (const auto& g : msdfData->Glyphs)
        {
            if (g.getCodepoint() == character)
            {
                glyph = &g;
                break;
            }
        }

        if (!glyph)
        {
            // Fallback to '?' glyph if not found
            for (const auto& g : msdfData->Glyphs)
            {
                if (g.getCodepoint() == '?')
                {
                    glyph = &g;
                    break;
                }
            }
            if (!glyph)
                continue;
        }

        // Get glyph quad bounds in atlas coordinates
        double al, ab, ar, at;
        glyph->getQuadAtlasBounds(al, ab, ar, at);
        glm::vec2 texCoordMin(static_cast<float>(al), static_cast<float>(ab));
        glm::vec2 texCoordMax(static_cast<float>(ar), static_cast<float>(at));

        // Adjust texture coordinates
        float texelWidth = 1.0f / 512;
        float texelHeight = 1.0f / 512;
        texCoordMin *= glm::vec2(texelWidth, texelHeight);
        texCoordMax *= glm::vec2(texelWidth, texelHeight);

        // Get glyph quad bounds in plane coordinates
        double pl, pb, pr, pt;
        glyph->getQuadPlaneBounds(pl, pb, pr, pt);
        glm::vec2 quadMin(static_cast<float>(pl), static_cast<float>(pb));
        glm::vec2 quadMax(static_cast<float>(pr), static_cast<float>(pt));

        // Scale and position the quad
        quadMin *= static_cast<float>(fsScale);
        quadMax *= static_cast<float>(fsScale);
        quadMin += glm::vec2(static_cast<float>(x), static_cast<float>(y));
        quadMax += glm::vec2(static_cast<float>(x), static_cast<float>(y));

        // Create four vertices for the glyph quad
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);
        vertices.push_back({glm::vec3(transform * glm::vec4(quadMin.x, quadMin.y, 0.0f, 1.0f)),
                            m_Material->Color,
                            texCoordMin,
                            textureIndex});

        vertices.push_back({glm::vec3(transform * glm::vec4(quadMin.x, quadMax.y, 0.0f, 1.0f)),
                            m_Material->Color,
                            glm::vec2(texCoordMin.x, texCoordMax.y),
                            textureIndex});

        vertices.push_back({glm::vec3(transform * glm::vec4(quadMax.x, quadMax.y, 0.0f, 1.0f)),
                            m_Material->Color,
                            texCoordMax,
                            textureIndex});

        vertices.push_back({glm::vec3(transform * glm::vec4(quadMax.x, quadMin.y, 0.0f, 1.0f)),
                            m_Material->Color,
                            glm::vec2(texCoordMax.x, texCoordMin.y),
                            textureIndex});

        // Indices
        std::vector<uint32_t> currentIndices = {0, 1, 2, 2, 3, 0};

        for (auto index : currentIndices)
        {
            indices.push_back(index + globalIndexOffset);
        }

        // Advance the x position
        double advance = glyph->getAdvance();

        // Apply kerning adjustment if available
        if (i + 1 < text.size())
        {
            char nextCharacter = text[i + 1];
            std::pair<int, int> kerningKey = {character, nextCharacter};
            auto kerningIt = kerningMap.find(kerningKey);
            if (kerningIt != kerningMap.end())
            {
                double kerning = kerningIt->second;
                advance += kerning;
            }
        }
        x += fsScale * advance;

        // Increment global index offset
        globalIndexOffset += 4;  // 4 vertices per character
    }
    // NOTE: Copy to real buffers data
    m_VerticesBytes.resize(vertices.size() * sizeof(TextVertex));
    memcpy(m_VerticesBytes.data(), vertices.data(), m_VerticesBytes.size());


    m_Indices.resize(indices.size() * sizeof(uint32_t));
    memcpy(m_Indices.data(), indices.data(), m_Indices.size());
}


}  // namespace Forge
