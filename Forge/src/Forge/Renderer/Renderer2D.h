//
// Created by toor on 2024-10-29.
//

#ifndef RENDERER2D_H
#define RENDERER2D_H

#include "Forge/Renderer/BatchManager.h"
#include "Forge/Renderer/Camera/Camera.h"
#include "Forge/Renderer/Font.h"
#include "Forge/Renderer/Material.h"
#include "Forge/Renderer/RenderableTargets/Quad.h"
#include "Forge/Renderer/UniformBuffer.h"
#include <cstdint>
#include <glm/glm.hpp>
#include <memory>

namespace Forge {

struct CombinedMatrices
{
    glm::mat4 ViewProjection;  // Matricea pentru obiectele din spațiul lumii
    glm::mat4 OrthoProjection;  // Matricea pentru obiectele din spațiul ecranului
};

struct QuadVertex
{
    glm::vec3 Position;
    glm::vec4 Color;
    glm::vec2 TexCoord;
    float TexIndex;  // New attribute for texture index
};

struct TextVertex
{
    glm::vec3 Position;
    glm::vec4 Color;
    glm::vec2 TexCoord;
    float TexIndex;
};


struct TextCache
{
    std::vector<TextVertex> vertices;
    std::vector<uint32_t> indices;
};

class Renderer2D
{
public:
    Renderer2D();
    ~Renderer2D();

    void BeginScene(const std::shared_ptr<Camera>& camera, uint32_t width, uint32_t height);
    void EndScene();

    // Primitives
    void DrawQuad(const glm::vec3& position,
                  const glm::vec2& size,
                  const std::shared_ptr<Material>& material);
    void DrawString(const std::string& text,
                    const glm::vec3& position,
                    const float scale,
                    const std::shared_ptr<Font>& font,
                    const std::shared_ptr<Material>& material);

private:
    std::unique_ptr<BatchManager<QuadVertex>> m_QuadBatch;
    std::vector<QuadVertex> m_StaticQuadVertices;
    std::vector<uint32_t> m_StaticQuadIndices;


    std::vector<QuadVertex> m_QuadVertices;
    const std::vector<uint32_t> quadIndices;


    std::unique_ptr<BatchManager<TextVertex>> m_TextBatch;

    std::unique_ptr<UniformBuffer> m_CombinedUniformBuffer;

    std::unordered_map<std::string, TextCache> m_TextCache;
};
}  // namespace Forge

#endif  // RENDERER2D_H
