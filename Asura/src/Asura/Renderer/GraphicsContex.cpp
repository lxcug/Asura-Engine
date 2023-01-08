#include "GraphicsContex.h"
#include "Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLContext.h"

Asura::Scope<Asura::GraphicsContext> Asura::GraphicsContext::createGraphicsContext(void *window) {
    switch (Asura::Renderer::getAPI()) {
        case RendererAPI::API::None:
            ASURA_CORE_ASSERT(false, "RendererAPI::API::None is currently not supported");
            return nullptr;
        case RendererAPI::API::OpenGL:
            return createScope<Asura::OpenGLContext>((GLFWwindow*)window);
    }
    ASURA_CORE_ASSERT(false, "Unknown RendererAPI");
    return nullptr;
}
