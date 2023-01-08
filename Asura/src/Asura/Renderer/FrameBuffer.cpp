#include "FrameBuffer.h"
#include "Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLFrameBuffer.h"


Asura::Ref<Asura::FrameBuffer> Asura::FrameBuffer::createFrameBuffer(const Asura::FrameBufferSpecification &spec) {
    switch (Asura::Renderer::getAPI()) {
        case RendererAPI::API::None:
            ASURA_CORE_ASSERT(false, "RendererAPI::API::None is currently not supported");
            return nullptr;
        case RendererAPI::API::OpenGL:
            return createRef<OpenGLFrameBuffer>(spec);
    }
    ASURA_CORE_ASSERT(false, "Unknown RendererAPI");
    return nullptr;
}
