//
// Created by toor on 2024-09-15.
//

#include "Editor.h"
#include "Forge/Core/Log/Log.h"
#include "Forge/Renderer/Camera/CameraController.h"
#include "glm/ext/matrix_transform.hpp"

#include <glm/fwd.hpp>
#include <memory>
#include <string>
#include <utility>

namespace Forge {

Editor::Editor() {}

Editor::~Editor() {}

void Editor::OnAttach() {
    RenderCommand::SetClearColor(glm::vec3(0.1));
    m_CameraScreenSpace = std::make_shared<Camera>(CameraProjection::ScreenSpaceCamera, glm::vec3(0.0, 0.0, 0.1));
    CameraController::Update(m_CameraScreenSpace);
    m_Camera = std::make_shared<Camera>(CameraProjection::PerspectiveCamera);
    CameraController::Update(m_Camera);

    m_Window = bf::Window::Create("First Window");
    m_Window->SetPosition({25, 25});
    m_Window->SetSize({300, 150});

    auto m_Button = bf::Button::Create({75, 35}, "Test");
    m_Button->SetPosition({300, 300});
    m_Button->SubscribeEvents([](bf::WidgetEvent event, bf::Widget& btn) {
        if (event == bf::WidgetEvent::ButtonRelease) {
            LOG_TRACE("Click");
        }
    });

    m_Button->SetPadding({8, 8, 8, 20});

    // clang-format off
    auto window = bf::Column::Create({
        bf::Row::Create({
            bf::Button::Create({100, 40}, "Create")
            ->SubscribeEvents([](bf::WidgetEvent event, bf::Widget& btn) {
                if (event == bf::WidgetEvent::ButtonRelease) {
                    LOG_TRACE("Create button clicked");
                }
            }),
            bf::Button::Create({100, 40}, "Delete")
            ->SubscribeEvents([](bf::WidgetEvent event, bf::Widget& btn) {
                if (event == bf::WidgetEvent::ButtonRelease) {
                    LOG_TRACE("Delete button clicked");
                }
            })
        }),

        bf::Row::Create({
            bf::Button::Create({100, 40}, "Load")
            ->SubscribeEvents([](bf::WidgetEvent event, bf::Widget& btn) {
                if (event == bf::WidgetEvent::ButtonRelease) {
                    LOG_TRACE("Load button clicked");
                }
            }),
            bf::Button::Create({100, 40}, "Quit")
            ->SubscribeEvents([](bf::WidgetEvent event, bf::Widget& btn) {
                if (event == bf::WidgetEvent::ButtonRelease) {
                    LOG_TRACE("Quit button clicked");
                }
            })
        })
        ->SetPadding({10, 10, 10, 0}),

    });
    // clang-format on
    m_Window->AddChild(window);
    m_Window->SetSize({233, 220});
    m_Renderer.AddUI(*m_Window);

    auto cube = Model("/home/toor/Downloads/scene.obj");
    m_Renderer.AddMesh(std::move(cube));

    m_Renderer.Begin();
    m_Renderer.Submit();
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
    m_Renderer.DrawMesh(m_Camera);
    m_Renderer.DrawUI(m_CameraScreenSpace);

    /*LOG_INFO("\n{}", m_CameraScreenSpace->GetProjectionMatrix())*/
    /*LOG_INFO("\n{}", m_Camera->GetProjectionMatrix())*/
}

// NOTE: Events
void Editor::OnEvent(const Event& event) {
    m_Window->OnEvent(event);
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
            m_CameraScreenSpace->RecalculateProjection(m_Width, m_Height);
        }
    }
}

} // namespace Forge
