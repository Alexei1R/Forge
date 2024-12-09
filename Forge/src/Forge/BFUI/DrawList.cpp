//
// Created by toor on 2024-11-27.
//

#include "DrawList.h"

#include "Forge/BFUI/Types.h"
#include <cstdint>
#include <cstring>

#include "Forge/Core/Log/Log.h"
#include "Forge/Renderer/Font.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"

namespace BfUI {

uint32_t DrawList::m_VerticesSize = 1000;
uint32_t DrawList::m_IndicesSize = 1000;


const DrawListData DrawList::DrawPanel(const vec2i position,
                                       const vec2i size,
                                       const vec4f color,
                                       float const textureIndex)
{
    // Now 'position' is the top-left corner of the panel.
    vec2i m_TopLeft = position;
    vec2i m_TopRight = vec2i(position.x + size.x, position.y);
    vec2i m_BottomLeft = vec2i(position.x, position.y + size.y);
    vec2i m_BottomRight = position + size;

    float type = (float)WidgetElementType::Panel;

    // UV coordinates are now set with the top at v = 0.0f and bottom at v = 1.0f
    std::vector<DrawListVertex> vertices = {
        {{m_TopLeft.x, m_TopLeft.y}, color, {0.0f, 0.0f}, textureIndex, size, type},
        {{m_TopRight.x, m_TopRight.y}, color, {1.0f, 0.0f}, textureIndex, size, type},
        {{m_BottomRight.x, m_BottomRight.y}, color, {1.0f, 1.0f}, textureIndex, size, type},
        {{m_BottomLeft.x, m_BottomLeft.y}, color, {0.0f, 1.0f}, textureIndex, size, type}};

    std::vector<uint32_t> indices = {0, 1, 2, 2, 3, 0};

    m_VerticesSize += vertices.size();
    m_IndicesSize += indices.size();

    std::vector<uint8_t> verticesBytes;
    verticesBytes.resize(vertices.size() * sizeof(DrawListVertex));
    std::memcpy(verticesBytes.data(), vertices.data(), verticesBytes.size());

    return {verticesBytes, indices};
}


// NOTE: Draw Text
const DrawListData DrawList::DrawText(const std::string& text,
                                      const vec2i position,
                                      const float scale,
                                      const vec4f color,
                                      const float textureIndex)

{
    const float epsilon = 0.001f;  // Example epsilon value

    const auto& font = Forge::Font::GetDefault();

    const Forge::MSDFData* msdfData = font->GetMSDFData();
    const auto& fontGeometry = msdfData->FontGeometry;
    const auto& metrics = fontGeometry.getMetrics();

    Handle atlasTextureHandle = font->GetAtlasTextureHandle();

    double x = 0.0;
    double y = 0.0;
    double fsScale = scale / (metrics.ascenderY - metrics.descenderY);
    double lineHeight = fsScale * metrics.lineHeight;

    const std::map<std::pair<int, int>, double>& kerningMap = fontGeometry.getKerning();


    std::vector<DrawListVertex> vertices;
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


        vec2i quadSize = {quadMax.x - quadMin.x, quadMax.y - quadMin.y};

        float type = (float)WidgetElementType::Text;
        // Create four vertices for the glyph quad
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(position, 1.0));
        vertices.push_back({glm::vec3(transform * glm::vec4(quadMin.x, quadMin.y, 0.0f, 1.0f)),
                            color,
                            texCoordMin,
                            textureIndex,
                            quadSize,
                            type});

        vertices.push_back({glm::vec3(transform * glm::vec4(quadMin.x, quadMax.y, 0.0f, 1.0f)),
                            color,
                            glm::vec2(texCoordMin.x, texCoordMax.y),
                            textureIndex,
                            quadSize,
                            type});

        vertices.push_back({glm::vec3(transform * glm::vec4(quadMax.x, quadMax.y, 0.0f, 1.0f)),
                            color,
                            texCoordMax,
                            textureIndex,
                            quadSize,
                            type});

        vertices.push_back({glm::vec3(transform * glm::vec4(quadMax.x, quadMin.y, 0.0f, 1.0f)),
                            color,
                            glm::vec2(texCoordMax.x, texCoordMin.y),
                            textureIndex,
                            quadSize,
                            type});

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


    m_VerticesSize += vertices.size();
    m_IndicesSize += indices.size();


    std::vector<uint8_t> verticesBytes;
    verticesBytes.resize(vertices.size() * sizeof(DrawListVertex));
    std::memcpy(verticesBytes.data(), vertices.data(), verticesBytes.size());
    return {verticesBytes, indices};
}


const DrawListData DrawList::DrawTextConstraind(
    const std::string& text,
    const vec2i position,
    const float scale,
    const vec4f color,
    const float textureIndex,
    vec2i constrains)
{
    vec2i textSize = {0, 0};
    int lineCount = 1;
    const float epsilon = 0.001f;  // Example epsilon value

    const auto& font = Forge::Font::GetDefault();

    const Forge::MSDFData* msdfData = font->GetMSDFData();
    const auto& fontGeometry = msdfData->FontGeometry;
    const auto& metrics = fontGeometry.getMetrics();

    Handle atlasTextureHandle = font->GetAtlasTextureHandle();

    double x = 0.0;
    double y = 0.0;
    double fsScale = scale / (metrics.ascenderY - metrics.descenderY);
    double lineHeight = fsScale * metrics.lineHeight;

    const std::map<std::pair<int, int>, double>& kerningMap = fontGeometry.getKerning();


    std::vector<DrawListVertex> vertices;
    vertices.reserve(4 * text.size());
    std::vector<uint32_t> indices;
    indices.reserve(6 * text.size());

    // Track global index offset
    uint32_t globalIndexOffset = 0;

    for (size_t i = 0; i < text.size(); ++i)
    {
        if (textSize.x > constrains.x - scale)
        {
            // Move to the next line
            x = 0;
            y -= lineHeight;
            textSize.x = 0;

            lineCount++;
        }


        textSize.x = std::max(textSize.x, static_cast<int>(x));
        textSize.y = static_cast<int>(lineCount * lineHeight);


        if (textSize.y > constrains.y - scale)
        {
            break;
        }

        char character = text[i];

        // Handle special characters
        if (character == '\r')
            continue;

        if (character == '\n')
        {
            x = 0;
            y -= lineHeight;

            lineCount++;
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


        vec2i quadSize = {quadMax.x - quadMin.x, quadMax.y - quadMin.y};

        float type = (float)WidgetElementType::Text;
        // Create four vertices for the glyph quad
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(position, 1.0));
        vertices.push_back({glm::vec3(transform * glm::vec4(quadMin.x, quadMin.y, 0.0f, 1.0f)),
                            color,
                            texCoordMin,
                            textureIndex,
                            quadSize,
                            type});

        vertices.push_back({glm::vec3(transform * glm::vec4(quadMin.x, quadMax.y, 0.0f, 1.0f)),
                            color,
                            glm::vec2(texCoordMin.x, texCoordMax.y),
                            textureIndex,

                            quadSize,
                            type});

        vertices.push_back({glm::vec3(transform * glm::vec4(quadMax.x, quadMax.y, 0.0f, 1.0f)),
                            color,
                            texCoordMax,
                            textureIndex,

                            quadSize,
                            type});


        vertices.push_back({glm::vec3(transform * glm::vec4(quadMax.x, quadMin.y, 0.0f, 1.0f)),
                            color,
                            glm::vec2(texCoordMax.x, texCoordMin.y),
                            textureIndex,

                            quadSize,
                            type});

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


    m_VerticesSize += vertices.size();
    m_IndicesSize += indices.size();


    std::vector<uint8_t> verticesBytes;
    verticesBytes.resize(vertices.size() * sizeof(DrawListVertex));
    std::memcpy(verticesBytes.data(), vertices.data(), verticesBytes.size());
    return {verticesBytes, indices};
}


const uint32_t DrawList::GetVerticesSize()
{
    return m_VerticesSize;
}
const uint32_t DrawList::GetIndicesSize()
{
    return m_IndicesSize;
}


}  // namespace BfUI
