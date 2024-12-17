
#ifndef UNIFORMBUFFER_H
#define UNIFORMBUFFER_H

#include <cstdint>
namespace Forge {

class UniformBuffer {
public:
    UniformBuffer(uint32_t size, uint32_t bindingPoint);

    void Submit(const void* data, uint32_t size, uint32_t offset = 0) const;

private:
    uint32_t m_RendererID;
};

} // namespace Forge

#endif
