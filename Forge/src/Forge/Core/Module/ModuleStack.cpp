
#include "ModuleStack.h"
#include <algorithm>

namespace Forge {

ModuleStack::ModuleStack() {}

ModuleStack::~ModuleStack() {}

void ModuleStack::PushModule(std::shared_ptr<Module> layer)
{
    m_Modules.emplace(m_Modules.begin() + m_ModuleInsertIndex, layer);
    m_ModuleInsertIndex++;
    layer->OnAttach();
}

void ModuleStack::PushOverlay(std::shared_ptr<Module> overlay)
{
    m_Modules.emplace_back(overlay);
    overlay->OnAttach();
}

void ModuleStack::PopModule(std::shared_ptr<Module> layer)
{
    auto it = std::find(m_Modules.begin(), m_Modules.begin() + m_ModuleInsertIndex, layer);
    if (it != m_Modules.begin() + m_ModuleInsertIndex)
    {
        (*it)->OnDetach();
        m_Modules.erase(it);
        m_ModuleInsertIndex--;
    }
}

void ModuleStack::PopOverlay(std::shared_ptr<Module> overlay)
{
    auto it = std::find(m_Modules.begin() + m_ModuleInsertIndex, m_Modules.end(), overlay);
    if (it != m_Modules.end())
    {
        (*it)->OnDetach();
        m_Modules.erase(it);
    }
}
}  // namespace Forge
