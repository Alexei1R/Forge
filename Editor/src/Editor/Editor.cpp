//
// Created by toor on 2024-09-15.
//

#include <glm/fwd.hpp>
#include <memory>
#include <string>

#include "Editor.h"
#include "Forge/BFUI/Button.h"
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
    BfUI::Theme::SetDarkTheme();
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

    button = BfUI::Button::Create(glm::vec2(50.0f, 50.0f), glm::vec2(100.0f, 50.0f), "Test");
    button->SubscribeEvents([](BfUI::WidgetEvents event, BfUI::Button& button) {
        if (event == BfUI::WidgetEvents::ButtonPress)
        {
            LOG_INFO("Button Pressed");
        }
    });


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


    button->Move(glm::vec2(50.0, m_Height - 100.0f));
    Renderer::SubmitUIElement(*button);

    std::string fps = std::format("{:.2f} ", fpsAverage);
    m_Text->Update(fps, {m_Width - 100.0f, m_Height - 80.0f, -0.1f}, 24);
    Renderer::SubmitText(*m_Text, materialManager->GetMaterial("DefaultText"));

    Renderer::End();
}

void Editor::OnEvent(const Event& event)
{
    for (auto& widget : BfUI::WidgetStack::GetAllWidgets())
    {
        widget->OnEvent(event);
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
            m_Width = windowEvent.GetX();
            m_Height = windowEvent.GetY();
            RenderCommand::SetViewport(0, 0, m_Width, m_Height);
            m_Camera->RecalculateProjection(m_Width, m_Height);
            m_CameraScreenSpace->RecalculateProjection(m_Width, m_Height);
        }
    }
}


}  // namespace Forge
