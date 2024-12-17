

#include "ForgeApplication.h"

#include <chrono>
#include <filesystem>
#include <memory>

#include "Forge/Core/Log/Log.h"
#include "Forge/Events/ImplEvent.h"
#include "Forge/Renderer/RenderCommand.h"
#include "Forge/Renderer/Renderer.h"
#include "Forge/Renderer/ShaderManager.h"
#include "Forge/Renderer/TextureManager.h"

namespace Forge {

Forge* Forge::s_Instance = nullptr;

Forge::Forge() {
    s_Instance = (Forge*)this;

    Log::Init("Forge");
    LOG_INFO("Hi form forge")
    SetPath();

    m_Window = std::make_shared<Window>();
    m_Window->SetEventCallback(std::bind(&Forge::HandleEvent, this, std::placeholders::_1));
    m_Window->EnableVSynk(false);

    m_GraphicsContext = std::make_unique<Context>(m_Window);
    RenderCommand::Init(m_Window);
}

Forge::~Forge() {}

void Forge::Run() {
    while (m_Running) {
        float deltaTime = m_Timer.getDeltaTime();

        for (auto component : m_ModuleStack) {
            component->OnUpdate(deltaTime);
        }

        m_GraphicsContext->SwapBuffers();
        m_Window->Update();
    }

    ShaderManager::GetInstance().Shutdown();
    TextureManager::GetInstance().Shutdown();
}

void Forge::HandleEvent(const Event& event) {
    for (auto component : m_ModuleStack) {
        component->OnEvent(event);
    }

    if (event.GetType() == EventType::Window) {
        WindowEvent windowEvent = static_cast<const WindowEvent&>(event);
        if (windowEvent.GetAction() == Action::Close) {
            LOG_INFO("Window Closed")
            m_Running = false;
        }
    }
};

void Forge::SetPath() {
    std::filesystem::path forgePath = std::filesystem::path(LIBRARY_DIR);
    try {
        std::filesystem::current_path(forgePath);
    } catch (const std::filesystem::filesystem_error& ex) {
        LOG_ERROR("Error setting current path:  {0}", ex.what());
    }
}

void Forge::PushModule(std::shared_ptr<Module> module) {
    m_ModuleStack.PushModule(module);
}

} // namespace Forge
