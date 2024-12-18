#ifndef MODEL_H
#define MODEL_H

#include <filesystem>
#include <vector>

#include "Forge/Core/Handle.h"
#include "Mesh.h"
#include "assimp/mesh.h"
#include "assimp/scene.h"

namespace Forge {

class Model : public MeshTarget {
public:
    Model(const std::filesystem::path& modelPath);

    const std::vector<Mesh>& GetMeshes() const override;
    const std::vector<Material>& GetMaterials() const override;

protected:
    void LoadModel(const std::filesystem::path& modelPath);

    void ProcessNode(aiNode* node, const aiScene* scene);
    void ProcessMesh(aiMesh* mesh, const aiScene* scene);

private:
    std::vector<Mesh> m_Meshes;
    std::vector<Material> m_Materials;
    std::filesystem::path m_BaseDir;
    uint32_t m_MeshIndicesOffset = 0;
    Handle shaderDefaultHandle;
};

} // namespace Forge

#endif
