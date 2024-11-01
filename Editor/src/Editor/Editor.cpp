//
// Created by toor on 2024-09-15.
//

#include "Editor.h"
#include "Forge/Renderer/Renderer2D.h"
#include "glm/fwd.hpp"
#include <iterator>
#include <memory>


namespace Forge {


Editor::Editor() {}

Editor::~Editor() {}

void Editor::OnAttach()
{
    RenderCommand::SetClearColor(glm::vec3(0.1));

    m_Camera = std::make_shared<EditorCamera>(glm::vec3(0.0f, 0.0f, 0.0f), 1920, 1080);
    m_Camera->SetCameraTarget(glm::vec3(0.0f, 0.0f, 0.0f));  // Look at origin
    Forge::GetForgeInstance().PushModule(m_Camera);

    m_Grid = std::make_shared<Grid>();
}

void Editor::OnDetach() {}


void Editor::OnUpdate(DeltaTime dt)
{
    RenderCommand::Clear();

    std::shared_ptr<Camera> camera = std::static_pointer_cast<Camera>(m_Camera);
    Renderer3D::BeginScene(camera);

    glm::mat4 transform = glm::mat4(1.0f);
    Renderer3D::DrawMesh(m_Grid->GetMesh(m_Camera), transform);

    Renderer3D::EndScene();


    renderer.BeginScene(camera);

    renderer.DrawQuad({0.0f, 0.0f, 0.5f}, {1.0f, 1.0f}, {0.0f, 1.0f, 1.0f, 1.0f});
    renderer.DrawQuad({0.5f, 0.5f, 0.0f}, {0.1f, 0.1f}, {0.0f, 1.0f, 0.5f, 1.0f});
    renderer.DrawQuad({0.0f, 0.0f, 0.0f}, {0.1f, 0.1f}, {1.0f, 0.0f, 0.0f, 1.0f});

    renderer.DrawQuad({0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}, {1.0f, 1.0f, 0.0f, 0.3f});
    renderer.EndScene();
}

void Editor::OnEvent(const Event& event)
{
    if (event.GetType() == EventType::Key)
    {
        KeyEvent keyEv = static_cast<const KeyEvent&>(event);
        if (keyEv.GetAction() == Action::KeyPress)
        {
        }
    }

    if (event.GetType() == EventType::Drop)
    {
        DropEvent dropEv = static_cast<const DropEvent&>(event);

        for (auto path : dropEv.GetFiles())
        {
            LOG_CRITICAL("Drop {}", path)
        }
    }


    if (event.GetType() == EventType::Window)
    {
        WindowEvent windowEvent = static_cast<const WindowEvent&>(event);
        if (windowEvent.GetAction() == Action::Resize)
        {
            RenderCommand::SetViewport(0, 0, windowEvent.GetX(), windowEvent.GetY());
            m_Camera->Resize(windowEvent.GetX(), windowEvent.GetY());
        }
    }
}


}  // namespace Forge
