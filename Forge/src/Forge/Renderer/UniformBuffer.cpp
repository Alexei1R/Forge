

#include "UniformBuffer.h"
#include "glad/glad.h"


namespace Forge {

UniformBuffer::UniformBuffer(uint32_t size, uint32_t bindingPoint)
{
    glGenBuffers(1, &m_RendererID);
    glBindBuffer(GL_UNIFORM_BUFFER, m_RendererID);
    glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, m_RendererID);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UniformBuffer::Submit(const void* data, uint32_t size, uint32_t offset) const
{
    glBindBuffer(GL_UNIFORM_BUFFER, m_RendererID);
    glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

}  // namespace Forge
