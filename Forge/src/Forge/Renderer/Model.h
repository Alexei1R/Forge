/*//*/
/*// Created by toor on 2024-11-30.*/
/*//*/
/**/
/*#ifndef MODEL_H*/
/*#define MODEL_H*/
/**/
/**/
/*#include <vector>*/
/*#include <filesystem>*/
/*#include "Mesh.h"*/
/**/
/**/
/*namespace Forge {*/
/**/
/**/
/*class Model : public MeshTarget*/
/*{*/
/*public:*/
/*    Model(const std::filesystem::path& modelPath);*/
/**/
/**/
/*    const std::vector<Mesh>& GetMeshes() const override;*/
/*    const std::vector<Material>& GetMaterials() const override;*/
/**/
/*protected:*/
/*    void LoadModel(const std::filesystem::path& modelPath);*/
/**/
/*private:*/
/*    std::vector<Mesh> m_Meshes;*/
/*    Handle shaderHandle;*/
/*    std::vector<Material> m_Materials;*/
/*};*/
/**/
/*}  // namespace Forge*/
/**/
/**/
/*#endif*/
