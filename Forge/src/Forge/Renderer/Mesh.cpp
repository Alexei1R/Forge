//
// Created by toor on 2024-09-23.
//


#include "Mesh.h"
#include "Forge/Renderer/Buffer.h"
#include "imgui.h"

namespace Forge {


Mesh::Mesh(const std::vector<Vertex>& vertices,
           const std::vector<unsigned int>& indices,
           const std::vector<TextureData>& textures) :
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

void Mesh::Draw(const std::shared_ptr<Shader>& shader)
{
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr = 1;
    unsigned int heightNr = 1;

    for (unsigned int i = 0; i < m_Textures.size(); i++)
    {
        m_Textures[i].texture->Bind(i);
        std::string number;
        std::string name = m_Textures[i].type;
        if (name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if (name == "texture_specular")
            number = std::to_string(specularNr++);
        else if (name == "texture_normal")
            number = std::to_string(normalNr++);
        else if (name == "texture_height")
            number = std::to_string(heightNr++);

        // Remove 'material.' prefix
        shader->SetUniform((name + number).c_str(), static_cast<int>(i));
    }

    // Draw mesh
    m_VAO->Bind();
    glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(m_Indices.size()), GL_UNSIGNED_INT, 0);
    m_VAO->Unbind();

    // Unbind textures
    for (unsigned int i = 0; i < m_Textures.size(); i++)
    {
        m_Textures[i].texture->UnBind();
    }
}


}  // namespace Forge
