// @author: lx
// @time: 2022/10/15
// @brief: 

#pragma once
#ifndef GAME_OPENGLCONTEXT_H
#define GAME_OPENGLCONTEXT_H

#include "Asura/Core/Core.h"
#include "Renderer/GraphicsContex.h"
#include "Glad/glad.h"
#include "GLFW/glfw3.h"
#include "GL/gl.h"
#include "Asura/Core/Log.h"

struct GLFWwindow;

namespace Asura {
    class ASURA_API OpenGLContext : public GraphicsContext {
    public:
        explicit OpenGLContext(GLFWwindow* windowHandle);

        void init() override;
        void swapBuffers() override;

    private:
        GLFWwindow* m_windowHandle;
    };
}


#endif //GAME_OPENGLCONTEXT_H
