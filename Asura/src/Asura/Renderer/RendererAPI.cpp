#include "RendererAPI.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

Asura::Scope<Asura::RendererAPI> Asura::RendererAPI::createRendererAPI() {
    switch (getAPI()) {
        case API::None:
            ASURA_CORE_ASSERT(false, "RendererAPI::API::None is currently not supported!");
            return nullptr;
        case API::OpenGL:
            return Asura::createScope<OpenGLRendererAPI>();
    }
    ASURA_CORE_ASSERT(false, "Unknown RendererAPI");
    return nullptr;
}
