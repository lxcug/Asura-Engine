#include "Texture.h"
#include "Core/Log.h"
#include "Renderer/RendererAPI.h"
#include "Platform/OpenGL/OpenGLTexture.h"


Asura::Ref<Asura::Texture2D> Asura::Texture2D::createTexture2D(const std::string &path) {
    switch (RendererAPI::getAPI()) {
        case RendererAPI::API::None:
            ASURA_CORE_ASSERT(false, "RendererAPI::API::None is currently not supported");
            return nullptr;
        case RendererAPI::API::OpenGL:
            return createRef<OpenGLTexture2D>(path);
    }
    return nullptr;
}

Asura::Ref<Asura::Texture2D> Asura::Texture2D::createTexture2D(uint32_t width, uint32_t height) {
    switch (RendererAPI::getAPI()) {
        case RendererAPI::API::None:
            ASURA_CORE_ASSERT(false, "RendererAPI::API::None is currently not supported");
            return nullptr;
        case RendererAPI::API::OpenGL:
            return createRef<OpenGLTexture2D>(width, height);
    }
    return nullptr;
}
