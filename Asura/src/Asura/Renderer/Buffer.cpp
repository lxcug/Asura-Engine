#include "Buffer.h"
#include "Renderer.h"
#include "Core/Log.h"
#include "Platform/OpenGL/OpenGLBuffer.h"


Asura::Ref<Asura::VertexBuffer> Asura::VertexBuffer::createVertexBuffer(unsigned size) {
    switch (RendererAPI::getAPI()) {
        case RendererAPI::API::None:
            ASURA_CORE_ASSERT(false, "RendererAPI::API::None is currently not supported!");
            return nullptr;
        case RendererAPI::API::OpenGL:
            return createRef<OpenGLVertexBuffer>(size);
    }
    ASURA_CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
}

Asura::Ref<Asura::VertexBuffer> Asura::VertexBuffer::createVertexBuffer(float *vertices, unsigned int size) {
    switch (RendererAPI::getAPI()) {
        case RendererAPI::API::None:
            ASURA_CORE_ASSERT(false, "RendererAPI::API::None is currently not supported!");
            return nullptr;
        case RendererAPI::API::OpenGL:
            return createRef<OpenGLVertexBuffer>(vertices, size);
    }
    ASURA_CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
}

Asura::Ref<Asura::IndexBuffer> Asura::IndexBuffer::createIndexBuffer(unsigned int *indices, unsigned int count) {
    switch (Renderer::getAPI()) {
        case RendererAPI::API::None:
            ASURA_CORE_ASSERT(false, "RendererAPI::API::None is currently not supported!");
            return nullptr;
        case RendererAPI::API::OpenGL:
            return createRef<OpenGLIndexBuffer>(indices, count);
    }
    ASURA_CORE_ASSERT(false, "Unknown RendererAPI::API!");
    return nullptr;
}
