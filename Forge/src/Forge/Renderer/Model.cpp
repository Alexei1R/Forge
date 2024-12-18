#include "Model.h"

#include <filesystem>
#include <vector>

#include "Forge/Core/Log/Log.h"
#include "Forge/Renderer/Mesh.h"
#include "Forge/Renderer/ShaderManager.h"
#include "Forge/Renderer/TextureManager.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"

namespace Forge {

static std::vector<std::string> GetAvailableImportExtensions();

Model::Model(const std::filesystem::path& modelPath) {
    if (modelPath.empty()) {
        LOG_WARN("Model path is empty.");
        return;
    }

    // NOTE: Check that import extension is available
    std::vector<std::string> extensions = GetAvailableImportExtensions();

    std::string fileExt = modelPath.extension().string();
    bool isExtensionSupported = std::find(extensions.begin(), extensions.end(), fileExt) != extensions.end();
    if (!isExtensionSupported) {
        LOG_WARN("Unsupported file extension: {}", fileExt);
        return;
    }

    auto& shaderManager = ShaderManager::GetInstance();
    shaderDefaultHandle = shaderManager.LoadShader(ShaderLayout{{ShaderType::VERTEX, "Assets/Shaders/Mesh/Default.vert"},
                                                                {ShaderType::FRAGMENT, "Assets/Shaders/Mesh/Default.frag"}});
    if (shaderDefaultHandle.GetValue() == 0) {
        LOG_CRITICAL("Failed to load shader.");
    }

    LoadModel(modelPath);
}

void Model::LoadModel(const std::filesystem::path& modelPath) {
    m_BaseDir = modelPath.parent_path();

    // NOTE: Importer
    Assimp::Importer importer;

    uint32_t importerFlags = aiProcess_Triangulate | aiProcess_FlipUVs;

    const aiScene* scene = importer.ReadFile(modelPath, importerFlags);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        LOG_ERROR("Error Assimp loading the model {}", importer.GetErrorString())
        return;
    }

    ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode* node, const aiScene* scene) {
    // NOTE: Process Node Meshes
    for (size_t i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        ProcessMesh(mesh, scene);
    }

    // NOTE:  Move to the next children in the tree
    for (size_t i = 0; i < node->mNumChildren; i++) {
        ProcessNode(node->mChildren[i], scene);
    }
}

void Model::ProcessMesh(aiMesh* mesh, const aiScene* scene) {
    Mesh localMesh;

    std::vector<uint32_t> indices;
    std::vector<MeshVertex> vertices;

    for (uint32_t i = 0; i < mesh->mNumVertices; i++) {
        MeshVertex vertex{};

        vertex.Position = {mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z};

        // Process normals
        if (mesh->HasNormals()) {
            vertex.Normal = glm::normalize(glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z));
        } else {
            vertex.Normal = {0.0f, 0.0f, 0.0f};
        }

        // Process texture coordinates
        if (mesh->HasTextureCoords(0)) {
            vertex.TexCoords = {mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y};
        } else {
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
        }

        vertices.push_back(vertex);
    }

    for (uint32_t i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (uint32_t j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    if (scene->HasMaterials() && mesh->mMaterialIndex >= 0) {
        aiMaterial* aiMaterial = scene->mMaterials[mesh->mMaterialIndex];

        Material material(shaderDefaultHandle);

        aiString materialName;
        aiMaterial->Get(AI_MATKEY_NAME, materialName);
        material.Name = materialName.C_Str();

        aiColor4D color;
        if (AI_SUCCESS == aiMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color)) {
            material.Color = glm::vec4(color.r, color.g, color.b, color.a);
        }

        aiColor4D specColor;
        if (AI_SUCCESS == aiMaterial->Get(AI_MATKEY_COLOR_SPECULAR, specColor)) {
            material.Specular = (specColor.r + specColor.g + specColor.b) / 3.0f;
        }

        // Extract emissive color
        aiColor4D emissiveColor;
        if (AI_SUCCESS == aiMaterial->Get(AI_MATKEY_COLOR_EMISSIVE, emissiveColor)) {
            material.EmissiveColor = glm::vec3(emissiveColor.r, emissiveColor.g, emissiveColor.b);
        }

        float shininess;
        if (AI_SUCCESS == aiMaterial->Get(AI_MATKEY_SHININESS, shininess)) {
            material.Roughness = 1.0f - (shininess / 100.0f);
        }

        /*// Process textures*/
        /*ProcessMaterialTextures(aiMaterial, material);*/

        localMesh.MaterialIndex = m_Materials.size();

        m_Materials.push_back(std::move(material));
    } else {
        localMesh.MaterialIndex = -1;
    }

    localMesh.Vertices = std::move(vertices);
    localMesh.Indices = std::move(indices);

    // Store the mesh
    m_Meshes.push_back(std::move(localMesh));
}

const std::vector<Mesh>& Model::GetMeshes() const {
    return m_Meshes;
}

const std::vector<Material>& Model::GetMaterials() const {
    return m_Materials;
}

static std::vector<std::string> GetAvailableImportExtensions() {
    std::vector<std::string> importersExt;
#ifdef FORGE_IMPORT_AVAILABLE_OBJ
    importersExt.push_back(".obj");
#endif
#ifdef FORGE_IMPORT_AVAILABLE_FBX
    importersExt.push_back(".fbx");
#endif
#ifdef FORGE_IMPORT_AVAILABLE_GLTF
    importersExt.push_back(".gltf");
#endif
#ifdef FORGE_IMPORT_AVAILABLE_COLLADA
    importersExt.push_back(".dae");
#endif
#ifdef FORGE_IMPORT_AVAILABLE_BLEND
    importersExt.push_back(".blend");
#endif
    return importersExt;
}

} // namespace Forge
