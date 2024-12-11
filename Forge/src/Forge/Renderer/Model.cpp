/*//*/
/*// Created by toor on 2024-11-30.*/
/*//*/
/**/
/*#include "Model.h"*/
/*#include "Forge/Core/Log/Log.h"*/
/*#include "Forge/Renderer/ShaderManager.h"*/
/*#include "Forge/Renderer/TextureManager.h"*/
/**/
/*#include <filesystem>*/
/**/
/**/
/*#define TINYOBJLOADER_IMPLEMENTATION*/
/*#include "tiny_obj_loader.h"*/
/**/
/**/
/*namespace Forge {*/
/**/
/**/
/*Model::Model(const std::filesystem::path& modelPath)*/
/*{*/
/*    auto& shaderManager = Forge::ShaderManager::GetInstance();*/
/*    shaderHandle = shaderManager.LoadShader(Forge::ShaderLayout {*/
/*        {Forge::ShaderType::VERTEX, "Assets/Shaders/Renderer3DMesh.vert"},*/
/*        {Forge::ShaderType::FRAGMENT, "Assets/Shaders/Renderer3DMesh.frag"}});*/
/*    if (shaderHandle.GetValue() == 0)*/
/*    {*/
/*        LOG_CRITICAL("Failed to load shader.");*/
/*    }*/
/**/
/*    if (modelPath.extension() != ".obj" || modelPath.empty())*/
/*    {*/
/*        LOG_WARN("Only .obj files are supported currently.");*/
/*        return;*/
/*    }*/
/**/
/**/
/*    LoadModel(modelPath);*/
/*}*/
/**/
/*void Model::LoadModel(const std::filesystem::path& modelPath)*/
/*{*/
/*    tinyobj::attrib_t attrib;*/
/*    std::vector<tinyobj::shape_t> shapes;*/
/*    std::vector<tinyobj::material_t> materials;*/
/*    std::string warn, err;*/
/**/
/*    std::filesystem::path baseDir = modelPath.parent_path();*/
/**/
/*    bool ret = tinyobj::LoadObj(*/
/*        &attrib,*/
/*        &shapes,*/
/*        &materials,*/
/*        &warn,*/
/*        &err,*/
/*        modelPath.c_str(),*/
/*        baseDir.c_str(),*/
/*        true);*/
/**/
/*    if (!warn.empty())*/
/*    {*/
/*        LOG_WARN("{}", warn)*/
/*    }*/
/*    if (!err.empty())*/
/*    {*/
/*        LOG_ERROR("{}", err)*/
/*    }*/
/*    if (!ret)*/
/*    {*/
/*        LOG_CRITICAL("Failed to load/parse .obj file:  {}", modelPath.string())*/
/*        return;*/
/*    }*/
/**/
/*    // Process materials*/
/*    for (size_t m = 0; m < materials.size(); m++)*/
/*    {*/
/*        const tinyobj::material_t& mat = materials[m];*/
/*        Material material(shaderHandle);*/
/*        material.Name = mat.name;*/
/*        material.Color = glm::vec4(mat.diffuse[0], mat.diffuse[1], mat.diffuse[2], 1.0f);*/
/*        material.Specular = mat.shininess;*/
/*        material.Metallic = mat.metallic;*/
/*        material.Roughness = mat.roughness;*/
/**/
/*        LOG_TRACE("Loaded New Material {}", mat.name)*/
/**/
/*        // Load textures if available*/
/*        if (!mat.diffuse_texname.empty())*/
/*        {*/
/*            std::filesystem::path texturePath = baseDir / mat.diffuse_texname;*/
/*            Handle textureHandle = TextureManager::GetInstance().LoadTexture(texturePath);*/
/*            material.SetTexture(0, textureHandle);*/
/*        }*/
/**/
/*        // Store the material*/
/*        m_Materials.push_back(material);*/
/*    }*/
/**/
/**/
/*    // Process shapes*/
/*    for (size_t s = 0; s < shapes.size(); s++)*/
/*    {*/
/*        tinyobj::shape_t& shape = shapes[s];*/
/**/
/*        // Create a Mesh*/
/*        Mesh mesh;*/
/**/
/*        size_t index_offset = 0;*/
/**/
/*        // For each face*/
/*        for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++)*/
/*        {*/
/*            size_t fv = shape.mesh.num_face_vertices[f];*/
/**/
/*            // Get the material ID for this face*/
/*            int material_id = shape.mesh.material_ids[f];*/
/*            if (material_id >= 0 && material_id < m_Materials.size())*/
/*            {*/
/*                mesh.MaterialIndex = material_id;*/
/*            }*/
/*            else*/
/*            {*/
/*                mesh.MaterialIndex = -1;  // Or a default material index*/
/*            }*/
/**/
/*            // For each vertex in the face*/
/*            for (size_t v = 0; v < fv; v++)*/
/*            {*/
/*                tinyobj::index_t idx = shape.mesh.indices[index_offset + v];*/
/*                MeshVertex vertex {};*/
/**/
/*                // Position*/
/*                vertex.Position = {attrib.vertices[3 * idx.vertex_index + 0],*/
/*                                   attrib.vertices[3 * idx.vertex_index + 1],*/
/*                                   attrib.vertices[3 * idx.vertex_index + 2]};*/
/**/
/*                // Normal*/
/*                if (idx.normal_index >= 0)*/
/*                {*/
/*                    vertex.Normal = {attrib.normals[3 * idx.normal_index + 0],*/
/*                                     attrib.normals[3 * idx.normal_index + 1],*/
/*                                     attrib.normals[3 * idx.normal_index + 2]};*/
/*                }*/
/*                else*/
/*                {*/
/*                    // Compute normal here if necessary*/
/*                    vertex.Normal = glm::vec3(0.0f, 0.0f, 0.0f);*/
/*                }*/
/**/
/*                // Texture Coordinates*/
/*                if (idx.texcoord_index >= 0)*/
/*                {*/
/*                    vertex.TexCoords = {attrib.texcoords[2 * idx.texcoord_index + 0],*/
/*                                        attrib.texcoords[2 * idx.texcoord_index + 1]};*/
/*                }*/
/*                else*/
/*                {*/
/*                    vertex.TexCoords = glm::vec2(0.0f, 0.0f);*/
/*                }*/
/**/
/*                // Add vertex directly without checking for duplicates*/
/*                mesh.Vertices.push_back(vertex);*/
/*                mesh.Indices.push_back(static_cast<uint32_t>(mesh.Vertices.size() - 1));*/
/*            }*/
/**/
/*            index_offset += fv;*/
/*        }*/
/**/
/*        // Store the mesh*/
/*        m_Meshes.push_back(mesh);*/
/*    }*/
/*}*/
/**/
/*const std::vector<Mesh>& Model::GetMeshes() const*/
/*{*/
/*    return m_Meshes;*/
/*}*/
/**/
/**/
/*const std::vector<Material>& Model::GetMaterials() const*/
/*{*/
/*    return m_Materials;*/
/*}*/
/*}  // namespace Forge*/
