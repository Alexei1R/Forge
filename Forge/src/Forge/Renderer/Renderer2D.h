//
// Created by toor on 2024-10-22.
//

#ifndef RENDERER2D_H
#define RENDERER2D_H

#include "Forge/Renderer/Camera/Camera.h"
#include "Forge/Renderer/RenderCommand.h"
#include <memory>
namespace Forge {


class Renderer2D
{
public:
    static void BeginScene(const std::shared_ptr<Camera>& camera);
    static void EndScene();


private:
    static std::shared_ptr<Camera> m_Camera;


    RenderCommand m_RenderCommand;
};


}  // namespace Forge


#endif
