//
// Created by toor on 2024-10-29.
//

#ifndef RENDERABLETARGET_H
#define RENDERABLETARGET_H


#include "Forge/Renderer/Buffer/Buffer.h"
#include "Forge/Renderer/Shader.h"
#include <cstddef>
#include <memory>
#include <vector>

namespace Forge {

template <typename VertexType>
class RenderableTarget
{
public:
    virtual const std::vector<VertexType>& GetVertices() const = 0;
    virtual const std::vector<unsigned int>& GetIndices(int offset) const = 0;
    virtual const std::shared_ptr<Shader>& GetShader() = 0;
    virtual const BufferLayout& GetBufferLayout() = 0;
    virtual ~RenderableTarget() = default;
};

}  // namespace Forge

#endif
