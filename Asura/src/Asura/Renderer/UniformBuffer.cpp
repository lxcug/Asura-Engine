#include "UniformBuffer.h"
#include "Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLUniformBuffer.h"


Asura::Ref<Asura::UniformBuffer> Asura::UniformBuffer::createUniformBuffer(uint32_t size, uint32_t binding) {
    switch (Asura::Renderer::getAPI()) {
        case RendererAPI::API::None: {
            ASURA_CORE_ASSERT(false, "RendererAPI::API::None is currently not supported!");
            return nullptr;
        }
        case RendererAPI::API::OpenGL: {
            return Asura::createRef<OpenGLUniformBuffer>(size, binding);
        }
    }
    ASURA_CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
}
