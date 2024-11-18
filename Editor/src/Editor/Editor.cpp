//
// Created by toor on 2024-09-15.
//

#include "Editor.h"
#include "Forge/Core/Log/Log.h"
#include "Forge/Renderer/Font.h"
#include "Forge/Renderer/Handle.h"
#include "Forge/Renderer/Renderer2D.h"
#include "Forge/Renderer/ShaderManager.h"
#include "glm/fwd.hpp"
#include <format>
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
    materialManager = std::make_shared<MaterialManager>();

    auto& shaderManager = ShaderManager::GetInstance();
    Handle shaderHandle = shaderManager.LoadShader(
        ShaderLayout {{ShaderType::VERTEX, "Assets/Shaders/Renderer2DText.vert"},
                      {ShaderType::FRAGMENT, "Assets/Shaders/Renderer2DText.frag"}});

    if (shaderHandle.GetValue() == 0)
    {
        LOG_CRITICAL("Failed to load shader.");
        return;  // Handle the failure case appropriately
    }


    auto textMaterial = materialManager->CreateMaterial("DefaultText", shaderHandle);
    textMaterial->Color = glm::vec4(1.0f);
    textMaterial->SetTexture(0, Font::GetDefault()->GetAtlasTextureHandle());
    textMaterial->SetParameter("u_UseScreenSpace", true);


    materialManager->Serialize();
    materialManager->Deserialize();
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

    std::shared_ptr<Camera> camera = std::static_pointer_cast<Camera>(m_Camera);


    renderer.BeginScene(camera, m_Width, m_Height);

    auto material = materialManager->GetMaterial("DefaultText");

    renderer.DrawQuad({0.0, 0.0, -1.0}, {100.0, 100.0}, material);
    std::string fps = std::format("{:.2f} ", fpsAverage);
    renderer.DrawString(fps,
                        {-(m_Width / 2) + 20, (m_Height / 2) - 40, -0.1f},
                        24,
                        Font::GetDefault(),
                        material);

    renderer.EndScene();


    /*Renderer::Submit();*/
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
            m_Width = windowEvent.GetX();
            m_Height = windowEvent.GetY();
            RenderCommand::SetViewport(0, 0, m_Width, m_Height);
            m_Camera->Resize(m_Width, m_Height);
        }
    }
}


}  // namespace Forge
