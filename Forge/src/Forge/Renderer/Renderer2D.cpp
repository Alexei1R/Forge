//
// Created by toor on 2024-10-22.
//

#include "Renderer2D.h"


namespace Forge {

struct TextVertex
{
    glm::vec3 Position;
    glm::vec4 Color;
    glm::vec2 TexCoord;
};


std::shared_ptr<Camera> Renderer2D::m_Camera = nullptr;


void Renderer2D::BeginScene(const std::shared_ptr<Camera>& camera)
{
    m_Camera = camera;
}
void Renderer2D::EndScene() {}


}  // namespace Forge
