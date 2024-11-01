//
// Created by toor on 2024-10-21.
//


#include "RenderCommand.h"
#include <cstdint>

namespace Forge {

glm::vec3 RenderCommand::m_ClearColor = glm::vec3(0.3, 0.3, 0.3);


RenderCommand::RenderCommand() {}

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

    glDrawElements(mode, buffer->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
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
        default:
            mode = GL_TRIANGLES;  // Default to triangles if unspecified
            break;
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
