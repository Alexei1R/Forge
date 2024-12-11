
#ifndef GRAPHICSCONTEXT_H
#define GRAPHICSCONTEXT_H

#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include <memory>

#include "Forge/Renderer/Window.h"

namespace Forge {

class Context
{
public:
    Context(std::shared_ptr<Window>& window);
    void SwapBuffers();

private:
    GLFWwindow* m_GLFWWindowPtr;
    std::shared_ptr<Window> m_Window;
};

}  // namespace Forge


#endif
