

#include "RenderCommand.h"
#include "Forge/Core/Log/Log.h"
#include "Forge/Renderer/RendererBatch.h"
#include "Forge/Renderer/Window.h"
#include <cstdint>

namespace Forge {

glm::vec3 RenderCommand::m_ClearColor = glm::vec3(0.3, 0.3, 0.3);
std::unordered_map<CursorType, GLFWcursor*> RenderCommand::m_Cursors;
std::shared_ptr<Window> RenderCommand::m_Window = nullptr;
void RenderCommand::Init(std::shared_ptr<Window>& window)
{
    m_Window = window;
    m_Cursors[CursorType::Arrow] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
    m_Cursors[CursorType::IBeam] = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
    m_Cursors[CursorType::Crosshair] = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
    m_Cursors[CursorType::Hand] = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
    m_Cursors[CursorType::HResize] = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
    m_Cursors[CursorType::VResize] = glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR);
    m_Cursors[CursorType::ResizeAll] = glfwCreateStandardCursor(GLFW_RESIZE_ALL_CURSOR);
}


void RenderCommand::SetCursorType(CursorType type)
{
    glfwSetCursor(static_cast<GLFWwindow*>(m_Window->GetNativeWindow()), m_Cursors[type]);
}

void RenderCommand::SetClearColor(glm::vec3 clearColor)
{
    m_ClearColor = clearColor;
    glClearColor(m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, 1.0f);
}

void RenderCommand::Clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void RenderCommand::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
    glViewport(x, y, width, height);
}


void RenderCommand::Draw(const std::shared_ptr<VertexArrayBuffer>& buffer,
                         DrawPrimitives primitives)
{
    buffer->Bind();

    GLenum mode;
    switch (primitives)
    {
        case DrawPrimitives::TRIANGLES: mode = GL_TRIANGLES; break;
        case DrawPrimitives::LINES: mode = GL_LINES; break;
        default:
            mode = GL_TRIANGLES;  // Default to triangles if unspecified
            break;
    }

    glDrawArrays(mode, 0, buffer->GetIndexBuffer()->GetCount());
}

void RenderCommand::DrawIndexed(const std::shared_ptr<VertexArrayBuffer>& buffer,
                                uint32_t indexCount,
                                DrawPrimitives primitives)
{
    buffer->Bind();

    GLenum mode;
    switch (primitives)
    {
        case DrawPrimitives::TRIANGLES: mode = GL_TRIANGLES; break;
        case DrawPrimitives::LINES: mode = GL_LINES; break;
        default: mode = GL_TRIANGLES; break;
    }

    glDrawElements(mode, indexCount, GL_UNSIGNED_INT, nullptr);
}

uint8_t RenderCommand::GetMaxTextureSlots()
{
    GLint maxTextureSlots;
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextureSlots);
    return static_cast<uint8_t>(maxTextureSlots);
}


}  // namespace Forge
