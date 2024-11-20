//
// Created by toor on 2024-10-29.
//

#ifndef RENDERABLETARGET_H
#define RENDERABLETARGET_H


#include "Forge/Renderer/Buffer/Buffer.h"
#include "Forge/Renderer/Shader.h"
#include <memory>
#include <vector>

namespace Forge {

class RenderableTarget
{
public:
    virtual const std::vector<uint8_t>& GetVertices() const = 0;
    virtual const std::vector<uint32_t>& GetIndices() const = 0;
    virtual const BufferLayout& GetLayout() const = 0;
    virtual ~RenderableTarget() = default;
    virtual uint32_t GetVerticesSize() const = 0;
    virtual const uint32_t GetIndicesSize() const = 0;
};

}  // namespace Forge

#endif
