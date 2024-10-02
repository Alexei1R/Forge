//
// Created by toor on 2024-08-25.
//

#ifndef COMPONENT_H
#define COMPONENT_H

#include "Forge/Core/Time.h"
#include "Forge/Events/Event.h"
namespace Forge {

enum class ComponentType
{
    None,
    Sensor,
    Plugin,
    Scene,
};
class Component
{
public:
    Component(const ComponentType ct = ComponentType::None);

    virtual ~Component() = default;

    virtual void OnAttach() {}

    virtual void OnDetach() {}

    virtual void OnUpdate(DeltaTime dt) {}

    virtual void OnEvent(const Event& event) {}

    virtual void OnImGuiRender() {}

    inline const Component GetComponentType()
    {
        return m_ComponentType;
    }

protected:
    ComponentType m_ComponentType;
};

}  // namespace Forge

#endif
