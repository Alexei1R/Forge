//
// Created by toor on 2024-10-15.
//

#ifndef GRID_H
#define GRID_H


#include <memory>
#include "Forge/Renderer/Mesh.h"
#include "Forge/Renderer/Shader.h"
#include "Forge/Renderer/EditorCamera.h"

namespace Forge {

class Grid
{
public:
    Grid();
    ~Grid();

    void OnAttach();
    void Draw(const std::shared_ptr<EditorCamera>& camera);

private:
    void SetupGrid();

    std::shared_ptr<Mesh> m_GridMesh;
    std::shared_ptr<Shader> m_GridShader;
};

}  // namespace Forge


#endif
