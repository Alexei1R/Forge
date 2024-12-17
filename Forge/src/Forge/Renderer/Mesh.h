
#ifndef MESH_H
#define MESH_H

#include <vector>

#include "Forge/Renderer/Buffer/Buffer.h"
#include "Forge/Renderer/Material.h"

namespace Forge {

class RenderableTarget {
public:
    virtual const std::vector<uint8_t>& GetVertices() const = 0;
    virtual const std::vector<uint32_t>& GetIndices() const = 0;
    virtual const BufferLayout& GetLayout() const = 0;
    virtual ~RenderableTarget() = default;
};

struct MeshVertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;

    bool operator==(const MeshVertex& other) const {
        return Position == other.Position && Normal == other.Normal && TexCoords == other.TexCoords;
    }
};

struct Mesh {
    std::vector<MeshVertex> Vertices;
    std::vector<uint32_t> Indices;
    int32_t MaterialIndex = -1;
};

class MeshTarget {
public:
    MeshTarget();

    virtual const std::vector<Mesh>& GetMeshes() const = 0;
    virtual const std::vector<Material>& GetMaterials() const = 0;

    virtual const BufferLayout& GetLayout() const;
    virtual ~MeshTarget() = default;

    static std::shared_ptr<Material> GetDefaultMaterial();

protected:
    BufferLayout m_Layout;
    static std::shared_ptr<Material> m_DefaultMaterial;
};

} // namespace Forge
#endif
