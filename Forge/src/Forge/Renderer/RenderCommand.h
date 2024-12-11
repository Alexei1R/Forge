
#ifndef RENDERCOMMAND_H
#define RENDERCOMMAND_H


#include "Forge/Renderer/Buffer/BufferImpl.h"
#include "Forge/Renderer/Material.h"
#include "Forge/Renderer/Window.h"
#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include "glm/glm.hpp"
#include <cstdint>
#include <memory>

namespace Forge {

enum class CursorType
{
    Arrow,
    IBeam,
    Crosshair,
    Hand,
    HResize,
    VResize,
    ResizeAll

};

class RenderCommand
{
public:
    static void Init(std::shared_ptr<Window>& window);
    static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
    static void SetClearColor(glm::vec3 clearColor);
    static void Clear();

    static void Draw(const std::shared_ptr<VertexArrayBuffer>& buffer,
                     DrawPrimitives primitives = DrawPrimitives::TRIANGLES);


    static void DrawIndexed(const std::shared_ptr<VertexArrayBuffer>& buffer,
                            uint32_t indexCount,
                            DrawPrimitives primitives = DrawPrimitives::TRIANGLES);

    static void SetCursorType(CursorType type);

    static uint8_t GetMaxTextureSlots();

private:
    static glm::vec3 m_ClearColor;
    static std::unordered_map<CursorType, GLFWcursor*> m_Cursors;
    static std::shared_ptr<Window> m_Window;
};

}  // namespace Forge


#endif
