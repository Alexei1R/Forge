
#include "DrawList.h"

#include <cstdint>
#include <cstring>

#include "Forge/BFUI/Types.h"
#include "Forge/Core/Log/Log.h"
#include "Forge/Renderer/Font.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"

namespace bf {

uint32_t DrawList::m_VerticesSize = 1000;
uint32_t DrawList::m_IndicesSize = 1000;

const DrawListData DrawList::DrawPanel(const vec2i position, const vec2i size, const vec4f color, float const textureIndex) {
    vec2i m_TopLeft = position;
    vec2i m_TopRight = vec2i(position.x + size.x, position.y);
    vec2i m_BottomLeft = vec2i(position.x, position.y + size.y);
    vec2i m_BottomRight = position + size;

    float type = (float)WidgetElementType::Panel;

    std::vector<DrawListVertex> vertices = {{{m_TopLeft.x, m_TopLeft.y}, color, {0.0f, 0.0f}, textureIndex, size, type},
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
const DrawListData DrawList::DrawText(const std::string& text, const vec2i position, const float scale, const vec4f color,
                                      const float textureIndex)

{
    std::vector<DrawListVertex> vertices;
    vertices.reserve(4 * text.size());
    std::vector<uint32_t> indices;
    indices.reserve(6 * text.size());
    const float type = (float)WidgetElementType::Text;

    // NOTE: Font Data
    const auto& font = Forge::Font::GetDefault();
    const Forge::MSDFData* msdfData = font->GetMSDFData();
    const auto& fontGeometry = msdfData->FontGeometry;
    const auto& metrics = fontGeometry.getMetrics();
    const std::map<std::pair<int, int>, double>& kerningMap = fontGeometry.getKerning();
    Handle atlasTextureHandle = font->GetAtlasTextureHandle();

    // TODO: LEARN WHAT THIS IS
    double x = 0.0;
    double y = 0.0;
    uint32_t caracterIndexOffset = 0;
    double fsScale = scale / (metrics.ascenderY - metrics.descenderY);
    double lineHeight = fsScale * metrics.lineHeight;

    for (size_t i = 0; i < text.size(); ++i) {
        char character = text[i];

        // Special cases
        if (character == '\r')
            continue;

        if (character == '\n') {
            x = 0;
            y -= lineHeight;
            continue;
        }

        if (character == ' ' || character == '\t') {
            const msdf_atlas::GlyphGeometry* glyph = nullptr;
            for (const auto& g : msdfData->Glyphs) {
                if (g.getCodepoint() == ' ') {
                    glyph = &g;
                    break;
                }
            }
            if (!glyph)
                continue;
            double advance = glyph->getAdvance();
            if (character == '\t')
                advance *= 4; // Handle tab as 4 spaces
            x += fsScale * advance;
            continue;
        }

        // Compute the quad from glyph
        msdf_atlas::unicode_t codepoint = (unsigned char)character;
        const msdf_atlas::GlyphGeometry* glyph = fontGeometry.getGlyph(codepoint);
        if (!glyph) {
            // WARN: Fallback to ? glyph
            LOG_WARN("Fallback to ? glyph")
            glyph = fontGeometry.getGlyph('?');
            if (!glyph)
                continue;
        }

        double al, ab, ar, at;
        glyph->getQuadAtlasBounds(al, ab, ar, at);
        glm::vec2 texCoordMin(static_cast<float>(al), static_cast<float>(at));
        glm::vec2 texCoordMax(static_cast<float>(ar), static_cast<float>(ab));
        // Adjust texture coordinates
        float texelWidth = 1.0f / 512;
        float texelHeight = 1.0f / 512;
        texCoordMin *= glm::vec2(texelWidth, texelHeight);
        texCoordMax *= glm::vec2(texelWidth, texelHeight);

        // Get glyph quad bounds in plane coordinates
        double pl, pb, pr, pt;
        glyph->getQuadPlaneBounds(pl, pb, pr, pt);
        glm::vec2 quadMin(static_cast<float>(pl), static_cast<float>(pt));
        glm::vec2 quadMax(static_cast<float>(pr), static_cast<float>(pb));

        // Scale and position the quad
        quadMin *= static_cast<float>(fsScale);
        quadMax *= static_cast<float>(fsScale);
        quadMin += glm::vec2(static_cast<float>(x), static_cast<float>(y));
        quadMax += glm::vec2(static_cast<float>(x), static_cast<float>(y));

        vec2i quadSize = {quadMin.x + quadMax.x, quadMax.y + quadMin.y};
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(position, 1.0));
        transform = glm::scale(transform, glm::vec3(1.0f, -1.0f, 1.0f));

        // NOTE: BOTTOM LEFT
        vertices.push_back({glm::vec3(transform * glm::vec4(quadMin.x, quadMax.y, 0.0f, 1.0f)), color,
                            vec2f(texCoordMin.x, texCoordMax.y), textureIndex, quadSize, type});
        // NOTE: BOTTOM RIGHT
        vertices.push_back(
            {glm::vec3(transform * glm::vec4(quadMax.x, quadMax.y, 0.0f, 1.0f)), color, texCoordMax, textureIndex, quadSize, type});
        // NOTE: TOP RIGHT
        vertices.push_back({glm::vec3(transform * glm::vec4(quadMax.x, quadMin.y, 0.0f, 1.0f)), color,
                            vec2f(texCoordMax.x, texCoordMin.y), textureIndex, quadSize, type});
        // NOTE: TOP LEFT
        vertices.push_back(
            {glm::vec3(transform * glm::vec4(quadMin.x, quadMin.y, 0.0f, 1.0f)), color, texCoordMin, textureIndex, quadSize, type});

        // NOTE: Indices
        std::vector<uint32_t> currentIndices = {0, 1, 2, 2, 3, 0};

        for (auto index : currentIndices) {
            indices.push_back(index + caracterIndexOffset);
        }

        // Advance the x position
        double advance = glyph->getAdvance();

        // Apply kerning adjustment if available
        if (i + 1 < text.size()) {
            char nextCharacter = text[i + 1];
            std::pair<int, int> kerningKey = {character, nextCharacter};
            auto kerningIt = kerningMap.find(kerningKey);
            if (kerningIt != kerningMap.end()) {
                double kerning = kerningIt->second;
                advance += kerning;
            }
        }
        x += fsScale * advance;
        caracterIndexOffset += 4;
    }

    //+= vectices
    m_VerticesSize += vertices.size();
    m_IndicesSize += indices.size();

    // Convert to bytes
    std::vector<uint8_t> verticesBytes;
    verticesBytes.resize(vertices.size() * sizeof(DrawListVertex));
    std::memcpy(verticesBytes.data(), vertices.data(), verticesBytes.size());
    return {verticesBytes, indices};
}

vec2f DrawList::MeasureText(const std::string& text, float scale) {
    // Initialize variables to track dimensions
    double totalWidth = 0.0;
    double maxWidth = 0.0;
    double totalHeight = 0.0;
    size_t currentLine = 1;

    // Retrieve font data
    const auto& font = Forge::Font::GetDefault();
    if (!font) {
        LOG_WARN("Default font not found.");
        return {0.0f, 0.0f};
    }

    const Forge::MSDFData* msdfData = font->GetMSDFData();
    if (!msdfData) {
        LOG_WARN("MSDF data not found in the default font.");
        return {0.0f, 0.0f};
    }

    const auto& fontGeometry = msdfData->FontGeometry;
    const auto& metrics = fontGeometry.getMetrics();
    const std::map<std::pair<int, int>, double>& kerningMap = fontGeometry.getKerning();

    double fsScale = scale / (metrics.ascenderY - metrics.descenderY);
    double lineHeight = fsScale * metrics.lineHeight;

    // Start with one line
    totalHeight = lineHeight;

    double x = 0.0;
    double y = 0.0;

    for (size_t i = 0; i < text.size(); ++i) {
        char character = text[i];

        // Handle special characters
        if (character == '\r') {
            continue; // Ignore carriage returns
        }

        if (character == '\n') {
            // Newline: Reset x, increment y, and update height
            if (x > maxWidth) {
                maxWidth = x;
            }
            x = 0.0;
            y -= lineHeight;
            totalHeight += lineHeight;
            currentLine++;
            continue;
        }

        if (character == ' ' || character == '\t') {
            // Handle space and tab
            const msdf_atlas::GlyphGeometry* glyph = nullptr;
            for (const auto& g : msdfData->Glyphs) {
                if (g.getCodepoint() == ' ') {
                    glyph = &g;
                    break;
                }
            }
            if (!glyph) {
                LOG_WARN("Space glyph not found.");
                continue;
            }
            double advance = glyph->getAdvance();
            if (character == '\t') {
                advance *= 4; // Treat tab as 4 spaces
            }
            x += fsScale * advance;
            continue;
        }

        // Get glyph for the character
        msdf_atlas::unicode_t codepoint = static_cast<unsigned char>(character);
        const msdf_atlas::GlyphGeometry* glyph = fontGeometry.getGlyph(codepoint);
        if (!glyph) {
            // Fallback to '?' glyph if character not found
            LOG_WARN("Glyph not found for character '%c'. Using fallback glyph '?'.", character);
            glyph = fontGeometry.getGlyph('?');
            if (!glyph) {
                LOG_WARN("Fallback glyph '?' not found.");
                continue;
            }
        }

        // Get advance width
        double advance = glyph->getAdvance();

        // Apply kerning if available
        if (i + 1 < text.size()) {
            char nextCharacter = text[i + 1];
            std::pair<int, int> kerningKey = {character, nextCharacter};
            auto kerningIt = kerningMap.find(kerningKey);
            if (kerningIt != kerningMap.end()) {
                double kerning = kerningIt->second;
                advance += kerning;
            }
        }

        // Accumulate the width
        x += fsScale * advance;
    }

    // After processing all characters, check the last line's width
    if (x > maxWidth) {
        maxWidth = x;
    }

    // Set totalWidth and totalHeight
    totalWidth = maxWidth;
    // totalHeight already accumulated based on lines

    return {static_cast<float>(totalWidth), static_cast<float>(totalHeight)};
}

const uint32_t DrawList::GetVerticesSize() {
    return m_VerticesSize;
}
const uint32_t DrawList::GetIndicesSize() {
    return m_IndicesSize;
}

} // namespace bf
