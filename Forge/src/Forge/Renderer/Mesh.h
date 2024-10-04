//
// Created by toor on 2024-09-23.
//

#ifndef MESH_H
#define MESH_H

#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include "BufferImpl.h"
#include "Forge/Renderer/Shader.h"
#include "Texture.h"


namespace Forge {


struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct TextureData
{
    std::shared_ptr<Texture> texture;
    std::string type;
    std::string path;
};

class Mesh
{
public:
    Mesh(const std::vector<Vertex>& vertices,
         const std::vector<unsigned int>& indices,
         const std::vector<TextureData>& textures = {});

    void Draw(const std::shared_ptr<Shader>& shader);

private:
    void SetupMesh();

    std::vector<Vertex> m_Vertices;
    std::vector<unsigned int> m_Indices;
    std::vector<TextureData> m_Textures;

    std::shared_ptr<VertexArrayBuffer> m_VAO;
    std::shared_ptr<VertexBuffer> m_VBO;
    std::shared_ptr<IndexBuffer> m_EBO;
};

}  // namespace Forge


#endif  // MESH_H
