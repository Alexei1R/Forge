//
// Created by toor on 2024-10-29.
//

#ifndef RENDERER2D_H
#define RENDERER2D_H

#include "Forge/Renderer/Buffer/BufferImpl.h"
#include "Forge/Renderer/Camera/Camera.h"
#include "Forge/Renderer/Shader.h"
#include <glm/glm.hpp>
#include <memory>

namespace Forge {

struct QuadVertex
{
    glm::vec3 Position;
    glm::vec4 Color;
};


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


    std::shared_ptr<VertexArrayBuffer> m_VAO;
    std::shared_ptr<VertexBuffer> m_VBO;
    std::shared_ptr<IndexBuffer> m_EBO;
    std::shared_ptr<Shader> m_Shader = nullptr;

    std::vector<QuadVertex> m_Vertices;
    std::vector<uint32_t> m_Indices;
};

}  // namespace Forge

#endif  // RENDERER2D_H
