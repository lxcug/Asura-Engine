#include "OpenGLContext.h"


Asura::OpenGLContext::OpenGLContext(GLFWwindow *windowHandle): m_windowHandle(windowHandle) {
    ASURA_CORE_ASSERT(windowHandle, "Window handle is nullptr");
}

void Asura::OpenGLContext::init() {
    ASURA_PROFILE_FUNCTION();
    glfwMakeContextCurrent(m_windowHandle);
    int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    ASURA_CORE_ASSERT(status, "Failed to initialize Glad");
}

void Asura::OpenGLContext::swapBuffers() {
    ASURA_PROFILE_FUNCTION();
    glfwSwapBuffers(m_windowHandle);
}
