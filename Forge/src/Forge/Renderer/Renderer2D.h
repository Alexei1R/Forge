//
// Created by toor on 2024-10-29.
//

#ifndef RENDERER2D_H
#define RENDERER2D_H

#include "Forge/Renderer/BatchManager.h"
#include "Forge/Renderer/Camera/Camera.h"
#include "Forge/Renderer/RenderableTargets/Quad.h"
#include <algorithm>
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
                  const glm::vec4& color = glm::vec4(1.0f));

private:
    glm::mat4 m_ViewProjectionMatrix;
    std::shared_ptr<Camera> m_Camera;


    std::unique_ptr<BatchManager<QuadVertex>> m_QuadBatch;
    std::shared_ptr<Shader> m_Shader = nullptr;
    Quad m_Quad;
};

}  // namespace Forge

#endif  // RENDERER2D_H
