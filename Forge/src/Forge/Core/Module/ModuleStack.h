
#ifndef MODULESTACK_H
#define MODULESTACK_H


#include <memory>
#include "Module.h"
namespace Forge {

class ModuleStack : public Module
{
public:
    ModuleStack();
    ~ModuleStack();

    void PushModule(std::shared_ptr<Module> component);
    void PushOverlay(std::shared_ptr<Module> overlay);
    void PopModule(std::shared_ptr<Module> component);
    void PopOverlay(std::shared_ptr<Module> overlay);

    std::vector<std::shared_ptr<Module>>::iterator begin()
    {
        return m_Modules.begin();
    }
    std::vector<std::shared_ptr<Module>>::iterator end()
    {
        return m_Modules.end();
    }

private:
    std::vector<std::shared_ptr<Module>> m_Modules;
    unsigned int m_ModuleInsertIndex = 0;
};

}  // namespace Forge


#endif
