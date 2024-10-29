//
// Created by toor on 2024-09-23.
//

#ifndef MESH_H
#define MESH_H

#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include "Forge/Renderer/Buffer/BufferImpl.h"
#include "Forge/Renderer/Texture.h"

namespace Forge {

struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

class Mesh
{
public:
    Mesh(const std::vector<Vertex>& vertices,
         const std::vector<unsigned int>& indices,
         const std::vector<std::shared_ptr<Texture>>& textures = {});


    std::shared_ptr<VertexArrayBuffer>& GetVertexArrayBuffer();
    std::vector<std::shared_ptr<Texture>>& GetTextures();

private:
    void SetupMesh();

    std::vector<Vertex> m_Vertices;
    std::vector<unsigned int> m_Indices;
    std::vector<std::shared_ptr<Texture>> m_Textures;

    std::shared_ptr<VertexArrayBuffer> m_VAO;
    std::shared_ptr<VertexBuffer> m_VBO;
    std::shared_ptr<IndexBuffer> m_EBO;
};

}  // namespace Forge


#endif  // MESH_H
