//
// Created by toor on 2024-09-15.
//

#include <glm/fwd.hpp>
#include <memory>
#include <string>

#include "Editor.h"
#include "Forge/BFUI/Button.h"
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

    m_Camera =
        std::make_shared<Camera>(CameraProjection::PerspectiveCamera, glm::vec3(0.0, 0.0, 5));

    CameraController::Update(m_Camera);


    m_CameraScreenSpace =
        std::make_shared<Camera>(CameraProjection::ScreenSpaceCamera, glm::vec3(0.0, 0.0, 1.0));

    CameraController::Update(m_CameraScreenSpace);
    Renderer::Initialize();


    materialManager = std::make_shared<MaterialManager>();

    auto& shaderManager = ShaderManager::GetInstance();
    Handle shaderHandle = shaderManager.LoadShader(
        ShaderLayout {{ShaderType::VERTEX, "Assets/Shaders/Renderer2DText.vert"},
                      {ShaderType::FRAGMENT, "Assets/Shaders/Renderer2DText.frag"}});
    if (shaderHandle.GetValue() == 0)
    {
        LOG_CRITICAL("Failed to load shader.");
    }
    auto textMaterial = materialManager->CreateMaterial("DefaultText", shaderHandle);
    textMaterial->Color = glm::vec4(1.0f);
    textMaterial->SetTexture(0, Font::GetDefault()->GetAtlasTextureHandle());
    textMaterial->Color = glm::vec4(1.0);
    m_Text = std::make_shared<Text>("", glm::vec3(0), 0, Font::GetDefault(), textMaterial);


    // NOTE: Draw Quad
    m_Quad = std::make_shared<Quad>(glm::vec3(0.0f, 0.0f, 0.0f),
                                    glm::vec2(1.0f, 1.0f),
                                    glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));


    Handle quadShader = shaderManager.LoadShader(
        ShaderLayout {{ShaderType::VERTEX, "Assets/Shaders/Renderer2DQuad.vert"},
                      {ShaderType::FRAGMENT, "Assets/Shaders/Renderer2DQuad.frag"}});
    if (quadShader.GetValue() == 0)
    {
        LOG_CRITICAL("Failed to load shader.");
    }
    auto quadMaterial = materialManager->CreateMaterial("QuadMaterial", quadShader);
    quadMaterial->Color = glm::vec4(1.0f);


    // NOTE: TEST the widgets
    // Button

    button =
        std::make_shared<BfUI::Button>(glm::vec2(50.0f, 50.0f), glm::vec2(100.0f, 50.0f), "Test");

    /*button.SetText("text")*/
    /*    .SetTextColor(glm::vec4(1.0))*/
    /*    .SetBackGroundColor(glm::vec4(1.0))*/
    /*    .SetBorderColor(glm::vec4(1.0))*/
    /*    .SetBackGroundColorHover(glm::vec4(1.0))*/
    /*    .SetBorderSize(32);*/
    /**/
    /*button.SubscribeEvents([](BfUI::BfUIEvents events, BfUI::Button& widget) {*/
    /*    widget.SetBackGroundColorActive(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));*/
    /*});*/


    /*materialManager->Serialize();*/
    /*materialManager->Deserialize();*/
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


    CameraController::Update(m_Camera);
    Renderer::Begin(m_Camera);

    Renderer::SubmitMesh(*m_Quad, materialManager->GetMaterial("QuadMaterial"));
    Renderer::End();


    // NOTE: Screen Space Camera
    Renderer::Begin(m_CameraScreenSpace);
    std::string fps = std::format("{:.2f} ", fpsAverage);

    auto mousePos = Mouse::GetMousePosition();
    m_Text->Update(fps, {200, 40, -0.1f}, 24);

    Renderer::SubmitUIElement(*button);
    Renderer::SubmitText(*m_Text, materialManager->GetMaterial("DefaultText"));
    Renderer::End();
}

void Editor::OnEvent(const Event& event)
{
    button->OnEvent(event);

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
            m_Camera->RecalculateProjection(m_Width, m_Height);
            m_CameraScreenSpace->RecalculateProjection(m_Width, m_Height);
        }
    }
}


}  // namespace Forge
