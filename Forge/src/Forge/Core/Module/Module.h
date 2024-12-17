#ifndef MODULE_H
#define MODULE_H

#include "Forge/Core/Time.h"
#include "Forge/Events/Event.h"
namespace Forge {

class Module {
public:
    Module() = default;
    virtual ~Module() = default;

    virtual void OnAttach() {}

    virtual void OnDetach() {}

    virtual void OnUpdate(DeltaTime dt) {}

    virtual void OnEvent(const Event& event) {}
};

} // namespace Forge

#endif
