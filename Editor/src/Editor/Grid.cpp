//
// Created by toor on 2024-10-15.
//

#include "Grid.h"
#include "Forge/Core/Log/Log.h"
#include "src/Editor/Editor.h"
#include <glm/gtc/type_ptr.hpp>

namespace Forge {

Grid::Grid()
{
    OnAttach();
}

Grid::~Grid() {}

void Grid::OnAttach()
{
    // Initialize shaders
    m_GridShader = std::make_shared<Shader>(std::initializer_list<ShaderElement> {
        {ShaderType::VERTEX, "Assets/Shaders/grid.vert"},
        {ShaderType::FRAGMENT, "Assets/Shaders/grid.frag"}});
    m_GridShader->Build();

    // Setup the grid mesh
    SetupGrid();
}

void Grid::SetupGrid()
{
    // Define plane vertices in NDC space (clip space)
    std::vector<Vertex> gridVertices = {
        // Positions (NDC coordinates), Normals (unused), TexCoords (unused)
        {{1.0f, 1.0f, 0.0f}, {}, {}},  // Top Right
        {{-1.0f, -1.0f, 0.0f}, {}, {}},  // Bottom Left
        {{-1.0f, 1.0f, 0.0f}, {}, {}},  // Top Left
        {{-1.0f, -1.0f, 0.0f}, {}, {}},  // Bottom Left
        {{1.0f, 1.0f, 0.0f}, {}, {}},  // Top Right
        {{1.0f, -1.0f, 0.0f}, {}, {}},  // Bottom Right
    };

    // Indices for the two triangles forming the quad
    std::vector<unsigned int> gridIndices = {0, 1, 2, 3, 4, 5};

    // Create the mesh
    m_GridMesh = std::make_shared<Mesh>(gridVertices, gridIndices);
}

void Grid::Draw(const std::shared_ptr<EditorCamera>& camera)
{
    m_GridShader->Bind();

    m_GridShader->SetUniform("view", camera->GetViewMatrix());
    m_GridShader->SetUniform("proj", camera->GetProjectionMatrix());

    m_GridMesh->Draw(m_GridShader);

    m_GridShader->UnBind();
}

}  // namespace Forge
