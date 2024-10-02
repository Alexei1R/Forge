//
// Created by toor on 2024-08-26.
//

#include <filesystem>
#include <memory>
#include "ForgeApplication.h"
#include "Forge/Components/GuiComponent.h"
#include "Forge/Events/ImplEvent.h"


namespace Forge {

Forge* Forge::s_Instance = nullptr;

Forge::Forge()
{
    s_Instance = (Forge*)this;

    Log::Init("Forge");
    LOG_INFO("Hi form forge")
    SetPath();

    m_Window = std::make_shared<Window>();
    m_Window->SetEventCallback(std::bind(&Forge::HandleEvent, this, std::placeholders::_1));
    m_Window->EnableVSynk(true);

    m_GraphicsContext = std::make_unique<Context>(m_Window);

    m_GuiComponent = std::make_shared<GuiComponent>(m_Window);
    PushComponent(m_GuiComponent);
}

Forge::~Forge() {}

void Forge::Run()
{
    while (m_Running)
    {
        float deltaTime = m_Timer.getDeltaTime();


        for (auto component : m_ComponentStack)
        {
            component->OnUpdate(deltaTime);
        }

        m_GuiComponent->Begin();
        for (auto component : m_ComponentStack)
        {
            component->OnImGuiRender();
        }
        m_GuiComponent->End();

        m_GraphicsContext->SwapBuffers();
        m_Window->Update();
    }
}


void Forge::HandleEvent(const Event& event)
{
    for (auto component : m_ComponentStack)
    {
        component->OnEvent(event);
    }

    if (event.GetType() == EventType::Window)
    {
        WindowEvent windowEvent = static_cast<const WindowEvent&>(event);
        if (windowEvent.GetAction() == Action::Close)
        {
            LOG_INFO("Window Closed")
            m_Running = false;
        }
        if (windowEvent.GetAction() == Action::Resize)
        {
            /*m_Renderer->SetViewport(0, 0, windowEvent.GetX(), windowEvent.GetY());*/
        }
    }
};


void Forge::SetPath()
{
    std::filesystem::path forgePath = std::filesystem::path(LIBRARY_DIR);
    try
    {
        std::filesystem::current_path(forgePath);
    }
    catch (const std::filesystem::filesystem_error& ex)
    {
        LOG_ERROR("Error setting current path:  {0}", ex.what());
    }
}


void Forge::PushComponent(std::shared_ptr<Component> component)
{
    m_ComponentStack.PushComponent(component);
}

}  // namespace Forge
