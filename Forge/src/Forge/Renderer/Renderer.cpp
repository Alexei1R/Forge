//
// Created by toor on 2024-10-21.
//

#include "Renderer.h"


namespace Forge {

std::shared_ptr<Camera> Renderer::m_Camera = nullptr;


void Renderer::BeginScene(const std::shared_ptr<Camera>& camera)
{
    m_Camera = camera;
}
void Renderer::EndScene() {}


void Renderer::DrawMesh(std::shared_ptr<Mesh>& mesh, glm::mat4 transform, DrawPrimitives primitives)
{
    auto vao = mesh->GetVertexArrayBuffer();
    vao->Bind();
    RenderCommand::Draw(vao, DrawPrimitives::LINES);
    vao->Unbind();
}

}  // namespace Forge
