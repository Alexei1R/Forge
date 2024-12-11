
#ifndef FORGEAPPLICATION_H
#define FORGEAPPLICATION_H

#include "Forge/Core/Log/Log.h"
#include "Forge/Core/Module/Module.h"
#include "Forge/Renderer/GraphicsContext.h"
#include "Forge/Renderer/Window.h"
#include "Forge/Core/Module/ModuleStack.h"
#include "Forge/Core/Time.h"
#include <memory>


namespace Forge {

class Forge
{
public:
    Forge();
    ~Forge();

    void Run();
    void PushModule(std::shared_ptr<Module> module);


    inline static Forge& GetForgeInstance()
    {
        return *s_Instance;
    }

    std::shared_ptr<Window> GetWindowPtr()
    {
        return m_Window;
    }


    inline ModuleStack& GetModulesStack()
    {
        return m_ModuleStack;
    }


private:
    void SetPath();
    void HandleEvent(const Event& event);


    static Forge* s_Instance;

    std::shared_ptr<Window> m_Window;
    std::unique_ptr<Context> m_GraphicsContext;


    Timer m_Timer;
    bool m_Running = true;
    ModuleStack m_ModuleStack;
};

}  // namespace Forge

#endif
