//
// Created by toor on 2024-10-21.
//

#ifndef RENDERCOMMAND_H
#define RENDERCOMMAND_H


#include "Forge/Renderer/Buffer/BufferImpl.h"
#include "glad/glad.h"
#include <cstdint>
#include <memory>

namespace Forge {

enum class DrawPrimitives : uint32_t
{
    POINTS,
    LINES,
    TRIANGLES,
};

enum class CullFace : uint32_t
{
    NONE = 0,
    FRONT,
    BACK,
    FRONT_AND_BACK
};

enum class BlendingMode : uint32_t
{
    NONE = 0,
    ADD,
    SUBTRACT,
    REVERSE_SUBTRACT
};

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
