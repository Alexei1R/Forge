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

template <typename VertexType>
class RenderableTarget
{
public:
    virtual std::vector<VertexType> GetVertices(const glm::vec3& position,
                                                const glm::vec2& size,
                                                const glm::vec4& color) const = 0;
    virtual std::vector<unsigned int> GetIndices() const = 0;
    virtual const BufferLayout& GetLayout() const = 0;
    virtual ~RenderableTarget() = default;
};

}  // namespace Forge

#endif
