
#include "GraphicsContext.h"


#include "Forge/Core/Log/Log.h"
#include "Forge/Core/Utils.h"

namespace Forge {

Context::Context(std::shared_ptr<Window>& window) : m_Window(window)
{
    LOG_INFO("Creating Graphics Context")
    m_GLFWWindowPtr = static_cast<GLFWwindow*>(m_Window->GetNativeWindow());

    F_ASSERT(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress),
             "Failed to initialize OpenGL context")

    LOG_INFO("OpenGL Info:");
    LOG_INFO("  Vendor: {}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
    LOG_INFO("  Renderer: {}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
    LOG_INFO("  Version: {}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
    int major = 0, minor = 0;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);
    LOG_INFO("  OpenGL Version: [{}.{}]", major, minor)

    glEnable(GL_DEPTH_TEST);


    glEnable(GL_BLEND);

    glDepthFunc(GL_LESS);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Context::SwapBuffers()
{
    // Swap buffers
    glfwSwapBuffers(m_GLFWWindowPtr);
}


}  // namespace Forge
