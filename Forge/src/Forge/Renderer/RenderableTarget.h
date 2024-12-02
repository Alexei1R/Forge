//
// Created by toor on 2024-10-29.
//

#ifndef RENDERABLETARGET_H
#define RENDERABLETARGET_H


#include "Forge/Renderer/Buffer/Buffer.h"
#include "Forge/Renderer/Material.h"
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
};


struct MeshVertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;

    bool operator==(const MeshVertex& other) const
    {
        return Position == other.Position && Normal == other.Normal && TexCoords == other.TexCoords;
    }
};

struct Mesh
{
    std::vector<MeshVertex> Vertices;
    std::vector<uint32_t> Indices;
    int32_t MaterialIndex = -1;
};

class MeshTarget
{
public:
    MeshTarget()
    {
        m_Layout = {{BufferDataType::Float3, "a_Position"},
                    {BufferDataType::Float3, "a_Normal"},
                    {BufferDataType::Float2, "a_TexCoords"}};
    }
    virtual const std::vector<Mesh>& GetMeshes() const = 0;
    virtual const std::vector<Material>& GetMaterials() const = 0;

    virtual const BufferLayout& GetLayout() const
    {
        return m_Layout;
    };
    virtual ~MeshTarget() = default;


protected:
    BufferLayout m_Layout;
};

}  // namespace Forge

#endif
