// Renderer2D.h
#ifndef RENDERER2D_H
#define RENDERER2D_H

#include "Forge/Renderer/Camera/Camera.h"
#include "Forge/Renderer/RenderCommand.h"
#include "Forge/Renderer/Shader.h"
#include "Forge/Renderer/Buffer/BufferImpl.h"
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
    void DrawQuad(const glm::vec2& position,
                  const glm::vec2& size,
                  const glm::vec4& color = glm::vec4(1.0f));

private:
    void Flush();
    void Init();
    void Shutdown();

    struct QuadVertex
    {
        glm::vec3 Position;
        glm::vec4 Color;
    };

    static constexpr uint32_t MaxQuads = 1000;
    static constexpr uint32_t MaxVertices = MaxQuads * 4;
    static constexpr uint32_t MaxIndices = MaxQuads * 6;

    std::shared_ptr<VertexArrayBuffer> m_VertexArray;
    std::shared_ptr<VertexBuffer> m_VertexBuffer;
    std::shared_ptr<IndexBuffer> m_IndexBuffer;
    std::shared_ptr<Shader> m_Shader;

    uint32_t m_QuadIndexCount = 0;
    QuadVertex* m_QuadVertexBufferBase = nullptr;
    QuadVertex* m_QuadVertexBufferPtr = nullptr;

    glm::mat4 m_ViewProjectionMatrix;
    std::shared_ptr<Camera> m_Camera;  // Store the camera for BeginScene
};

}  // namespace Forge

#endif  // RENDERER2D_H
