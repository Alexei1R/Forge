//
// Created by toor on 2024-09-15.
//

#ifndef GUICOMPONENT_H
#define GUICOMPONENT_H


#include <memory>
#include "GLFW/glfw3.h"
#include "Forge/Renderer/Window.h"
#include "Forge/Core/Components/Component.h"

#include <imgui.h>


namespace Forge {

class GuiComponent : public Component
{
public:
    GuiComponent(std::shared_ptr<Window> window);

    void OnAttach() override;
    void OnDetach() override;
    void OnUpdate(DeltaTime dt) override;
    void OnEvent(const Event& event) override;
    void OnImGuiRender() override;

    void Begin();
    void End();

private:
    void SetDarkThemeColors();


private:
    GLFWwindow* m_GLFWWindowPtr;
    std::shared_ptr<Window> m_Window;
};


}  // namespace Forge


#endif
