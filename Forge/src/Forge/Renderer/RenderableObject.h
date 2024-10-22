//
// Created by toor on 2024-10-21.
//

#ifndef RENDERABLEOBJECT_H
#define RENDERABLEOBJECT_H


#include "Forge/Renderer/Buffer/BufferImpl.h"
#include "Forge/Renderer/Texture.h"
#include <memory>
#include <vector>
namespace Forge {


class RenderableObject
{
public:
    virtual std::shared_ptr<VertexArrayBuffer>& GetVertexArrayBuffer() = 0;
    virtual std::vector<std::shared_ptr<Texture>>& GetTextures() = 0;
};


}  // namespace Forge


#endif
