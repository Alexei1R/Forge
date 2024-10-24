//
// Created by toor on 2024-10-22.
//

#ifndef RENDERER2D_H
#define RENDERER2D_H

#include "Forge/Renderer/Camera/Camera.h"
#include "Forge/Renderer/RenderCommand.h"
#include "Forge/Renderer/Shader.h"
#include <memory>
namespace Forge {


class Renderer2D
{
public:
    static void BeginScene(const std::shared_ptr<Camera>& camera);
    static void EndScene();


    // Primitives
    static void DrawQuad();

private:
    RenderCommand m_RenderCommand;
};


}  // namespace Forge


#endif
