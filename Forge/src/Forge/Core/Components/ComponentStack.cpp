//
// Created by toor on 2024-08-25.
//

#include "ComponentStack.h"
#include <algorithm>

namespace Forge {

ComponentStack::ComponentStack() {}

ComponentStack::~ComponentStack() {}

void ComponentStack::PushComponent(std::shared_ptr<Component> layer) {
    m_Components.emplace(m_Components.begin() + m_ComponentInsertIndex, layer);
    m_ComponentInsertIndex++;
    layer->OnAttach();
}

void ComponentStack::PushOverlay(std::shared_ptr<Component> overlay) {
    m_Components.emplace_back(overlay);
    overlay->OnAttach();
}

void ComponentStack::PopComponent(std::shared_ptr<Component> layer) {
    auto it = std::find(m_Components.begin(), m_Components.begin() + m_ComponentInsertIndex, layer);
    if (it != m_Components.begin() + m_ComponentInsertIndex) {
        (*it)->OnDetach();
        m_Components.erase(it);
        m_ComponentInsertIndex--;
    }
}

void ComponentStack::PopOverlay(std::shared_ptr<Component> overlay) {
    auto it = std::find(m_Components.begin() + m_ComponentInsertIndex, m_Components.end(), overlay);
    if (it != m_Components.end()) {
        (*it)->OnDetach();
        m_Components.erase(it);
    }
}
}  // namespace Forge
