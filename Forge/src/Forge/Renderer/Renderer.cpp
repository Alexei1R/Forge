//
// Created by toor on 2024-09-11.
//

#include "Renderer.h"


namespace Forge {

Renderer::Renderer() {}

void Renderer::SetClearColor(glm::vec3 clearColor)
{
    m_ClearColor = clearColor;
}

void Renderer::Clear()
{
    glClearColor(m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void Renderer::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
    glViewport(x, y, width, height);
}


void Renderer::Draw(std::shared_ptr<IndexBuffer>& buffer, DrawPrimitives primitives)
{
    switch (primitives)
    {
        case DrawPrimitives::TRIANGLES:
            glDrawElements(GL_TRIANGLES, buffer->GetCount(), GL_UNSIGNED_INT, 0);
            break;

        case DrawPrimitives::LINES:
            glDrawElements(GL_LINES, buffer->GetCount(), GL_UNSIGNED_INT, 0);
            break;
    }
}
}  // namespace Forge
