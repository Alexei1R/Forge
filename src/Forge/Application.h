//
// Created by toor on 2024-07-30 .
//

#ifndef APPLICATION_H
#define APPLICATION_H

#include <algorithm>
#include <memory>
#include "Forge/Renderer/Window.h"

namespace Forge {
class Application {
   public:
    Application();
    ~Application();
    void Run();

   private:
    void HandleEvent(const Event &event);

   private:
    std::shared_ptr<Window> m_Window;
};
}  // namespace Forge

#endif
