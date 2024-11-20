//
// Created by toor on 2024-10-21.
//

#ifndef RENDERCOMMAND_H
#define RENDERCOMMAND_H


#include "Forge/Renderer/Buffer/BufferImpl.h"
#include "Forge/Renderer/Material.h"
#include "glad/glad.h"
#include "glm/glm.hpp"
#include <cstdint>
#include <memory>

namespace Forge {

class RenderCommand
{
public:
    RenderCommand();

    static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
    static void SetClearColor(glm::vec3 clearColor);
    static void Clear();

    static void Draw(const std::shared_ptr<VertexArrayBuffer>& buffer,
                     DrawPrimitives primitives = DrawPrimitives::TRIANGLES);


    static void DrawIndexed(const std::shared_ptr<VertexArrayBuffer>& buffer,
                            uint32_t indexCount,
                            DrawPrimitives primitives = DrawPrimitives::TRIANGLES);


    static uint8_t GetMaxTextureSlots();

private:
    static glm::vec3 m_ClearColor;
};

}  // namespace Forge


#endif
