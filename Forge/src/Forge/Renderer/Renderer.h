//
// Created by toor on 2024-09-11.
//

#ifndef RENDERER_H
#define RENDERER_H

#include "Forge/Renderer/BufferImpl.h"
#include "glad/glad.h"
#include "glm/glm.hpp"
#include <memory>

namespace Forge {

enum class DrawPrimitives : unsigned int
{
    POINTS = 0x0000,
    LINES = 0x0001,
    LINE_LOOP = 0x0002,
    LINE_STRIP = 0x0003,
    TRIANGLES = 0x0004,
    TRIANGLE_STRIP = 0x0005,
    TRIANGLE_FAN = 0x0006
};

class Renderer
{
public:
    Renderer();

    void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
    void SetClearColor(glm::vec3 clearColor);
    void Clear();

    static void Draw(std::shared_ptr<IndexBuffer>& buffer,
                     DrawPrimitives primitives = DrawPrimitives::TRIANGLES);

private:
    glm::vec3 m_ClearColor = glm::vec3(0.2, 0.2, 0.2f);
};

}  // namespace Forge
#endif
