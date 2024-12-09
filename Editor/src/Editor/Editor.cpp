//
// Created by toor on 2024-09-15.
//

#include <glm/fwd.hpp>
#include <memory>
#include <string>

#include "Editor.h"
#include "Forge/BFUI/WidgetStack.h"
#include "Forge/Core/Log/Log.h"
#include "Forge/Events/Event.h"
#include "Forge/Events/ImplEvent.h"
#include "Forge/Renderer/Camera/Camera.h"
#include "Forge/Renderer/Camera/CameraController.h"

namespace Forge {


Editor::Editor() {}

Editor::~Editor() {}

void Editor::OnAttach()
{
    RenderCommand::SetClearColor(glm::vec3(0.1));


    m_CameraScreenSpace =
        std::make_shared<Camera>(CameraProjection::ScreenSpaceCamera, glm::vec3(0.0, 0.0, 0.1));

    CameraController::Update(m_CameraScreenSpace);
    Renderer::Initialize();


    m_Window = BfUI::Window::Create("First Window");
}

void Editor::OnDetach() {}

void Editor::OnUpdate(DeltaTime dt)
{
    timeAccumulator += dt;
    frameCount++;

    if (timeAccumulator >= 0.1f)
    {
        fpsAverage = frameCount / timeAccumulator;
        frameCount = 0;
        timeAccumulator = 0.0f;
    }


    RenderCommand::Clear();


    // NOTE: Screen Space Camera
    Renderer::Begin(m_CameraScreenSpace);


    Renderer::SubmitUIElement(*m_Window);

    Renderer::End();
}

void Editor::OnEvent(const Event& event)
{
    m_Window->OnEvent(event);
    m_Window->Update();


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
            m_Width = windowEvent.GetX();
            m_Height = windowEvent.GetY();
            RenderCommand::SetViewport(0, 0, m_Width, m_Height);
            m_CameraScreenSpace->RecalculateProjection(m_Width, m_Height);
        }
    }
}


}  // namespace Forge
