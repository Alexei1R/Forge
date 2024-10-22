//
// Created by toor on 2024-10-21.
//

#include "Renderer3D.h"


namespace Forge {

std::shared_ptr<Camera> Renderer3D::m_Camera = nullptr;


void Renderer3D::BeginScene(const std::shared_ptr<Camera>& camera)
{
    m_Camera = camera;
}
void Renderer3D::EndScene() {}


void Renderer3D::DrawMesh(std::shared_ptr<Mesh>& mesh,
                          glm::mat4 transform,
                          DrawPrimitives primitives)
{
    auto vao = mesh->GetVertexArrayBuffer();
    vao->Bind();
    RenderCommand::Draw(vao, DrawPrimitives::LINES);
    vao->Unbind();
}

}  // namespace Forge
