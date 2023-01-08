#include "VertexArray.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

Asura::Ref<Asura::VertexArray> Asura::VertexArray::createVertexArray() {
    switch (RendererAPI::getAPI()) {
        case RendererAPI::API::None:
            ASURA_CORE_ASSERT(false, "RendererAPI::API::None is currently not supported!");
            return nullptr;
        case RendererAPI::API::OpenGL:
            return createRef<Asura::OpenGLVertexArray>();
    }
    return nullptr;
}
