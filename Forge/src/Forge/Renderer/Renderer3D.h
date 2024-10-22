//
// Created by toor on 2024-10-21.
//

#ifndef RENDERER_H
#define RENDERER_H

#include "Forge/Renderer/Camera/Camera.h"
#include "Forge/Renderer/Model/Mesh.h"
#include "Forge/Renderer/RenderCommand.h"
#include <memory>
namespace Forge {


class Renderer3D
{
public:
    static void BeginScene(const std::shared_ptr<Camera>& camera);
    static void EndScene();


    static void DrawMesh(std::shared_ptr<Mesh>& mesh,
                         glm::mat4 transform,
                         DrawPrimitives primitives = DrawPrimitives::TRIANGLES);


private:
    static std::shared_ptr<Camera> m_Camera;


    RenderCommand m_RenderCommand;
};


}  // namespace Forge


#endif
