//
// Created by toor on 2024-08-25.
//

#ifndef COMPONENTSTACK_H
#define COMPONENTSTACK_H

#include <memory>
#include "Component.h"
namespace Forge {

class ComponentStack : public Component {
   public:
    ComponentStack();
    ~ComponentStack();

    void PushComponent(std::shared_ptr<Component> component);
    void PushOverlay(std::shared_ptr<Component> overlay);
    void PopComponent(std::shared_ptr<Component> component);
    void PopOverlay(std::shared_ptr<Component> overlay);

    std::vector<std::shared_ptr<Component>>::iterator begin() { return m_Components.begin(); }
    std::vector<std::shared_ptr<Component>>::iterator end() { return m_Components.end(); }

   private:
    std::vector<std::shared_ptr<Component>> m_Components;
    unsigned int m_ComponentInsertIndex = 0;
};

}  // namespace Forge

#endif
