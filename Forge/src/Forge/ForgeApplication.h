//
// Created by toor on 2024-08-26.
//

#ifndef FORGEAPPLICATION_H
#define FORGEAPPLICATION_H

#include "Forge/Core/Log/Log.h"
#include "Forge/Core/Components/Component.h"
#include "Forge/Renderer/GraphicsContext.h"
#include "Forge/Renderer/Window.h"
#include "Forge/Components/GuiComponent.h"
#include "Forge/Core/Components/ComponentStack.h"
#include "Forge/Core/Time.h"
#include <memory>


namespace Forge {

class Forge
{
public:
    Forge();
    ~Forge();

    void Run();
    void PushComponent(std::shared_ptr<Component> component);


    inline static Forge& GetForgeInstance()
    {
        return *s_Instance;
    }

    std::shared_ptr<Window> GetWindowPtr()
    {
        return m_Window;
    }

private:
    void SetPath();
    void HandleEvent(const Event& event);


    static Forge* s_Instance;

    std::shared_ptr<Window> m_Window;
    std::unique_ptr<Context> m_GraphicsContext;
    std::shared_ptr<GuiComponent> m_GuiComponent;


    Timer m_Timer;
    bool m_Running = true;
    ComponentStack m_ComponentStack;
};

}  // namespace Forge

#endif
