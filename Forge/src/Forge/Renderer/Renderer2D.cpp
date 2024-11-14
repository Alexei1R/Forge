//
// Created by toor on 2024-10-29.
//

#include "Renderer2D.h"
#include "Forge/Core/Log/Log.h"
#include "Forge/Renderer/BatchManager.h"
#include "Forge/Renderer/RenderCommand.h"
#include "Forge/Renderer/RenderableTargets/Quad.h"
#include "Forge/Renderer/UniformBuffer.h"
#include "glm/fwd.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <algorithm>
#include <cstdint>
#include <memory>
#include <random>
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

#include "Forge/Renderer/TextureManager.h"


namespace Forge {

Renderer2D::Renderer2D()
{
    // NOTE: Uniform Buffer for Camera, Do not remove


    auto mat4size = sizeof(glm::mat4);
    LOG_INFO("{}", mat4size)
    m_CombinedUniformBuffer =
        std::make_unique<UniformBuffer>(sizeof(CombinedMatrices), 0);  // Binding 0


    // Slightly higher capacity to avoid frequent flushing
    BatchProps props(
        100,
        4,
        6,
        {{BufferDataType::Float3, "a_Position"},
         {BufferDataType::Float4, "a_Color"},
         {BufferDataType::Float2, "a_TextCoord"},
         {BufferDataType::Float, "a_Slot"}});
    m_QuadBatch = std::make_unique<BatchManager<QuadVertex>>(props);


    BatchProps textBatchProps(
        255,
        4,
        6,
        {{BufferDataType::Float3, "a_Position"},
         {BufferDataType::Float4, "a_Color"},
         {BufferDataType::Float2, "a_TexCoord"},
         {BufferDataType::Float, "a_TexIndex"}});
    m_TextBatch = std::make_unique<BatchManager<TextVertex>>(textBatchProps);

    LOG_INFO("MaxTextureSlots {}", RenderCommand::GetMaxTextureSlots())
}

Renderer2D::~Renderer2D() {}

void Renderer2D::BeginScene(const std::shared_ptr<Camera>& camera, uint32_t width, uint32_t height)
{
    CombinedMatrices matrices;
    matrices.ViewProjection = camera->GetViewProjectionMatrix();


    float aspectRatio = static_cast<float>(width) / static_cast<float>(height);
    float orthoHeight = 10.0f;
    float orthoWidth = orthoHeight * aspectRatio;
    matrices.OrthoProjection = glm::ortho(
        -orthoWidth / 2.0f,
        orthoWidth / 2.0f,
        -orthoHeight / 2.0f,
        orthoHeight / 2.0f,
        0.001f,
        1000.0f);

    m_CombinedUniformBuffer->Submit(&matrices, sizeof(CombinedMatrices));
    m_QuadBatch->BeginBatch();
    m_TextBatch->BeginBatch();
}

void Renderer2D::EndScene()
{
    m_QuadBatch->EndBatch();
    m_TextBatch->EndBatch();
}

void Renderer2D::DrawQuad(const glm::vec3& position,
                          const glm::vec2& size,
                          const std::shared_ptr<Material>& material)


{
    // Precompute transformation matrix
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
                          glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

    // Static local-space vertices for a unit quad
    static const std::array<glm::vec4, 4> localVertices = {
        glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f),  // Bottom-left
        glm::vec4(0.5f, -0.5f, 0.0f, 1.0f),  // Bottom-right
        glm::vec4(0.5f, 0.5f, 0.0f, 1.0f),  // Top-right
        glm::vec4(-0.5f, 0.5f, 0.0f, 1.0f)  // Top-left
    };

    // Corresponding texture coordinates for each vertex
    static const std::array<glm::vec2, 4> textureCoords = {
        glm::vec2(0.0f, 0.0f),  // Bottom-left
        glm::vec2(1.0f, 0.0f),  // Bottom-right
        glm::vec2(1.0f, 1.0f),  // Top-right
        glm::vec2(0.0f, 1.0f)  // Top-left
    };


    // Transform vertices and apply color, texture coordinates, and texture index
    std::vector<QuadVertex> transformedVertices;
    transformedVertices.reserve(4);
    for (size_t i = 0; i < localVertices.size(); ++i)
    {
        glm::vec3 worldPosition = glm::vec3(transform * localVertices[i]);
        transformedVertices.push_back({
            worldPosition,  // Position
            material->Color,  // Color
            textureCoords[i],  // TexCoord
            static_cast<float>(0)  // TexIndex
        });
    }

    // Predefined index order (two triangles forming the quad)
    static const std::vector<uint32_t> quadIndices = {0, 1, 2, 2, 3, 0};

    m_QuadBatch->Submit(transformedVertices, quadIndices, material);
}


void Renderer2D::DrawString(const std::string& text,
                            const glm::vec3& position,
                            const float scale,
                            const std::shared_ptr<Font>& font,
                            const std::shared_ptr<Material>& material)
{
    // Get MSDF data from the font
    const MSDFData* msdfData = font->GetMSDFData();
    const auto& fontGeometry = msdfData->FontGeometry;
    const auto& metrics = fontGeometry.getMetrics();

    // Get the font atlas texture using the texture handle
    Handle atlasTextureHandle = font->GetAtlasTextureHandle();
    float textureIndex = material->GetTexureSlot(atlasTextureHandle);
    // Initialize position and scaling
    double x = 0.0;
    double y = 0.0;
    double fsScale = scale / (metrics.ascenderY - metrics.descenderY);
    double lineHeight = fsScale * metrics.lineHeight;

    // Retrieve the kerning map
    const std::map<std::pair<int, int>, double>& kerningMap = fontGeometry.getKerning();

    // Loop through characters in the text
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

        if (character == ' ')
        {
            // Handle space character advance
            // Get the glyph for space character
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
            x += fsScale * advance;
            continue;
        }

        if (character == '\t')
        {
            // Handle tab character (assuming 4 spaces)
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
            double advance = glyph->getAdvance() * 4;
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
            // Use the glyph for '?' as a fallback
            for (const auto& g : msdfData->Glyphs)
            {
                if (g.getCodepoint() == '?')
                {
                    glyph = &g;
                    break;
                }
            }
            if (!glyph)
                continue;  // Skip if no fallback glyph is found
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

        // Construct the transform matrix
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);
        /*glm::scale(glm::mat4(1.0f), glm::vec3(scale));*/

        // Create four vertices for the glyph quad
        std::vector<TextVertex> vertices;
        vertices.reserve(4);

        vertices.push_back({glm::vec3(transform * glm::vec4(quadMin.x, quadMin.y, 0.0f, 1.0f)),
                            material->Color,
                            texCoordMin,
                            textureIndex});

        vertices.push_back({glm::vec3(transform * glm::vec4(quadMin.x, quadMax.y, 0.0f, 1.0f)),
                            material->Color,
                            glm::vec2(texCoordMin.x, texCoordMax.y),
                            textureIndex});

        vertices.push_back({glm::vec3(transform * glm::vec4(quadMax.x, quadMax.y, 0.0f, 1.0f)),
                            material->Color,
                            texCoordMax,
                            textureIndex});

        vertices.push_back({glm::vec3(transform * glm::vec4(quadMax.x, quadMin.y, 0.0f, 1.0f)),
                            material->Color,
                            glm::vec2(texCoordMax.x, texCoordMin.y),
                            textureIndex});

        // Predefined index order (two triangles forming the quad)
        std::vector<uint32_t> indices = {0, 1, 2, 2, 3, 0};

        // Submit the vertices and indices to the text batch
        m_TextBatch->Submit(vertices, indices, material);

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
    }
}


}  // namespace Forge
