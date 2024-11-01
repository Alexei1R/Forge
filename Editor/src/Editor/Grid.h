//
// Created by toor on 2024-10-21.
//

#ifndef GRID_H
#define GRID_H

#include <memory>
#include <vector>
#include <glm/glm.hpp>

#include "Forge.h"


namespace Forge {

class Grid
{
public:
    Grid();
    ~Grid();

    void OnAttach();


    std::shared_ptr<Mesh>& GetMesh(const std::shared_ptr<EditorCamera>& camera);

private:
    void SetupGrid();

    std::shared_ptr<Mesh> m_GridMesh;
    std::shared_ptr<Shader> m_GridShader;
};

}  // namespace Forge

#endif
