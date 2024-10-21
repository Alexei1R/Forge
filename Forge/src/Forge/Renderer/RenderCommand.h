//
// Created by toor on 2024-10-21.
//

#ifndef RENDERCOMMAND_H
#define RENDERCOMMAND_H


#include "Forge/Renderer/BufferImpl.h"
#include "glad/glad.h"
#include "glm/glm.hpp"
#include <memory>

namespace Forge {

enum class DrawPrimitives : unsigned int
{
    /*POINTS = 0x0000,*/
    LINES = 0x0001,
    /*LINE_LOOP = 0x0002,*/
    /*LINE_STRIP = 0x0003,*/
    TRIANGLES = 0x0004,
    /*TRIANGLE_STRIP = 0x0005,*/
    /*TRIANGLE_FAN = 0x0006*/
};

class RenderCommand
{
public:
    RenderCommand();

    static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
    static void SetClearColor(glm::vec3 clearColor);
    static void Clear();

    static void Draw(std::shared_ptr<VertexArrayBuffer>& buffer,
                     DrawPrimitives primitives = DrawPrimitives::TRIANGLES);

private:
    static glm::vec3 m_ClearColor;
};

}  // namespace Forge


#endif
