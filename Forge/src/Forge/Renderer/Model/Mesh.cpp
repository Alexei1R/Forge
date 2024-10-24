//
// Created by toor on 2024-09-23.
//


#include "Mesh.h"
#include "Forge/Renderer/Buffer/BufferImpl.h"


namespace Forge {

Mesh::Mesh(const std::vector<Vertex>& vertices,
           const std::vector<unsigned int>& indices,
           const std::vector<std::shared_ptr<Texture>>& textures) :
    m_Vertices(vertices), m_Indices(indices), m_Textures(textures)
{
    SetupMesh();
}

void Mesh::SetupMesh()
{
    m_VAO = std::make_shared<VertexArrayBuffer>();
    m_VBO = std::make_shared<VertexBuffer>(m_Vertices.data(), m_Vertices.size() * sizeof(Vertex));
    m_EBO = std::make_shared<IndexBuffer>(m_Indices.data(), m_Indices.size());

    m_VAO->Bind();
    m_VBO->Bind();

    BufferLayout layout = {{BufferDataType::Float3, "aPos"},
                           {BufferDataType::Float3, "aNormal"},
                           {BufferDataType::Float2, "aTexCoords"}};

    m_VBO->SetLayout(layout);
    m_VAO->AddVertexBuffer(m_VBO);

    m_EBO->Bind();
    m_VAO->SetIndexBuffer(m_EBO);

    m_VAO->Unbind();
}

std::shared_ptr<VertexArrayBuffer>& Mesh::GetVertexArrayBuffer()
{
    return m_VAO;
}
std::vector<std::shared_ptr<Texture>>& Mesh::GetTextures()
{
    return m_Textures;
}


}  // namespace Forge
