//
// Created by toor on 2024-10-20.
//

#include "Grid.h"

namespace Forge {

Grid::Grid()
{
    OnAttach();
}

Grid::~Grid() {}

void Grid::OnAttach()
{
    std::initializer_list<ShaderElement> shaderElements = {
        {ShaderType::VERTEX, "Assets/Shaders/grid.vert"},
        {ShaderType::FRAGMENT, "Assets/Shaders/grid.frag"}};

    m_GridShader = std::make_shared<Shader>(shaderElements);
    m_GridShader->Build();

    SetupGrid();
}

void Grid::SetupGrid()
{
    std::vector<Vertex> gridVertices;
    std::vector<unsigned int> gridIndices;

    float gridSize = 10.0f;  // Total grid size (10 meters)
    int gridLines = 20;  // Number of lines per axis
    float halfSize = gridSize / 2.0f;
    float step = gridSize / gridLines;

    unsigned int index = 0;

    // Lines parallel to the X-axis
    for (int i = 0; i <= gridLines; ++i)
    {
        float z = -halfSize + i * step;

        // Start point
        Vertex startVertex;
        startVertex.Position = glm::vec3(-halfSize, 0.0f, z);
        startVertex.Normal = glm::vec3(0.0f, 1.0f, 0.0f);
        startVertex.TexCoords = glm::vec2(0.0f, 0.0f);
        gridVertices.push_back(startVertex);

        // End point
        Vertex endVertex;
        endVertex.Position = glm::vec3(halfSize, 0.0f, z);
        endVertex.Normal = glm::vec3(0.0f, 1.0f, 0.0f);
        endVertex.TexCoords = glm::vec2(0.0f, 0.0f);
        gridVertices.push_back(endVertex);

        // Indices
        gridIndices.push_back(index);
        gridIndices.push_back(index + 1);

        index += 2;
    }

    // Lines parallel to the Z-axis
    for (int i = 0; i <= gridLines; ++i)
    {
        float x = -halfSize + i * step;

        // Start point
        Vertex startVertex;
        startVertex.Position = glm::vec3(x, 0.0f, -halfSize);
        startVertex.Normal = glm::vec3(0.0f, 1.0f, 0.0f);
        startVertex.TexCoords = glm::vec2(0.0f, 0.0f);
        gridVertices.push_back(startVertex);

        // End point
        Vertex endVertex;
        endVertex.Position = glm::vec3(x, 0.0f, halfSize);
        endVertex.Normal = glm::vec3(0.0f, 1.0f, 0.0f);
        endVertex.TexCoords = glm::vec2(0.0f, 0.0f);
        gridVertices.push_back(endVertex);

        // Indices
        gridIndices.push_back(index);
        gridIndices.push_back(index + 1);

        index += 2;
    }

    m_GridMesh = std::make_shared<Mesh>(gridVertices, gridIndices);
}
std::shared_ptr<Mesh>& Grid::GetMesh(const std::shared_ptr<EditorCamera>& camera)
{
    m_GridShader->Bind();

    // Set transformation matrices
    glm::mat4 model = glm::mat4(1.0f);  // Identity matrix

    m_GridShader->SetUniform("model", model);
    m_GridShader->SetUniform("view", camera->GetViewMatrix());
    m_GridShader->SetUniform("proj", camera->GetProjectionMatrix());


    return m_GridMesh;
}


}  // namespace Forge
