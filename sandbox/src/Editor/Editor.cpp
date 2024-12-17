//
// Created by toor on 2024-09-15.
//

#include "Editor.h"
#include "Forge/Core/Log/Log.h"
#include "Forge/Renderer/Camera/CameraController.h"

#include <glm/fwd.hpp>
#include <memory>
#include <string>
#include <utility>

namespace Forge {

Editor::Editor() {}

Editor::~Editor() {}

void Editor::OnAttach() {
    RenderCommand::SetClearColor(glm::vec3(0.1));
    m_Camera = std::make_shared<Camera>(CameraProjection::PerspectiveCamera, glm::vec3(0.0, 0.0, 5));
    CameraController::Update(m_Camera);

    /*auto cube = Model("/home/toor/Downloads/monkey.obj");*/
    /*m_Renderer.AddMesh(std::move(cube));*/
    /**/
    /*auto tor = Model("/home/toor/Downloads/tor.obj");*/
    /*m_Renderer.AddMesh(std::move(tor));*/
    /**/
    auto monkey = Model("/home/toor/Downloads/82-koenigsegg-agera/uploads_files_2792345_Koenigsegg.obj");
    m_Renderer.AddMesh(std::move(monkey));

    /*auto sponza = Model("/home/toor/Downloads/sponza/sponza.obj");*/
    /*m_Renderer.AddMesh(std::move(sponza));*/
    m_Renderer.Begin();
}

void Editor::OnDetach() {}

void Editor::OnUpdate(DeltaTime dt) {
    timeAccumulator += dt;
    frameCount++;

    if (timeAccumulator >= 0.3f) {
        fpsAverage = frameCount / timeAccumulator;
        frameCount = 0;
        timeAccumulator = 0.0f;
        std::string fps = std::format("{:.2f} ", fpsAverage);
        LOG_INFO("{}", fps)
    }

    CameraController::Update(m_Camera);
    RenderCommand::Clear();
    m_Renderer.Draw(m_Camera);
}

// NOTE: Events
void Editor::OnEvent(const Event& event) {
    if (event.GetType() == EventType::Drop) {
        DropEvent dropEv = static_cast<const DropEvent&>(event);

        for (auto path : dropEv.GetFiles()) {
            LOG_CRITICAL("Drop {}", path)
        }
    }

    if (event.GetType() == EventType::Window) {
        WindowEvent windowEvent = static_cast<const WindowEvent&>(event);
        if (windowEvent.GetAction() == Action::Resize) {
            m_Width = windowEvent.GetX();
            m_Height = windowEvent.GetY();
            RenderCommand::SetViewport(0, 0, m_Width, m_Height);
            m_Camera->RecalculateProjection(m_Width, m_Height);
        }
    }
}

} // namespace Forge
