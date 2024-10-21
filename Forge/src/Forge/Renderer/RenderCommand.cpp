//
// Created by toor on 2024-10-21.
//


#include "RenderCommand.h"

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


void RenderCommand::Draw(std::shared_ptr<VertexArrayBuffer>& buffer, DrawPrimitives primitives)
{
    switch (primitives)
    {
        case DrawPrimitives::TRIANGLES:

            glDrawElements(GL_TRIANGLES, buffer->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, 0);
            break;


        case DrawPrimitives::LINES:
            glDrawElements(GL_LINES, buffer->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, 0);
            break;
    }
}
}  // namespace Forge
