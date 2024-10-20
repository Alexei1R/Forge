//
// Created by toor on 2024-09-15.
//

#include "Editor.h"
#include <memory>


namespace Forge {

Editor::Editor() {}

Editor::~Editor() {}

void Editor::OnAttach()
{
    m_Renderer = std::make_shared<Renderer>();
    m_Renderer->SetClearColor(glm::vec3(0.3f, 0.3f, 0.3f));

    m_Camera = std::make_shared<EditorCamera>(glm::vec3(0.0f, 0.0f, 0.0f), 1920, 1080);
    m_Camera->SetCameraTarget(glm::vec3(0.0f, 0.0f, 0.0f));  // Look at origin
    Forge::GetForgeInstance().PushModule(m_Camera);


    m_Grid = std::make_shared<Grid>();
}

void Editor::OnDetach() {}

void Editor::OnUpdate(DeltaTime dt)
{
    m_Renderer->Clear();

    m_Grid->Draw(m_Camera);
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
            m_Renderer->SetViewport(0, 0, windowEvent.GetX(), windowEvent.GetY());
            m_Camera->Resize(windowEvent.GetX(), windowEvent.GetY());
        }
    }
}


}  // namespace Forge
