//
// Created by toor on 2024-10-29.
//

#ifndef RENDERER2D_H
#define RENDERER2D_H

#include "Forge/Renderer/BatchManager.h"
#include "Forge/Renderer/Camera/Camera.h"
#include "Forge/Renderer/Material.h"
#include "Forge/Renderer/RenderableTargets/Quad.h"
#include <glm/glm.hpp>
#include <memory>

namespace Forge {

class Renderer2D
{
public:
    Renderer2D();
    ~Renderer2D();

    void BeginScene(const std::shared_ptr<Camera>& camera);
    void EndScene();

    // Primitives
    void DrawQuad(const glm::vec3& position,
                  const glm::vec2& size,
                  const std::shared_ptr<Material>& material);

private:
    glm::mat4 m_ViewProjectionMatrix;
    std::shared_ptr<Camera> m_Camera;

    std::unique_ptr<BatchManager<QuadVertex>> m_QuadBatch;
    std::vector<QuadVertex> m_StaticQuadVertices;
    std::vector<uint32_t> m_StaticQuadIndices;


    std::vector<QuadVertex> m_QuadVertices;
    const std::vector<uint32_t> quadIndices;
};
}  // namespace Forge

#endif  // RENDERER2D_H
