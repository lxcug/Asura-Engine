#include "Shader.h"
#include "Platform/OpenGL/OpenGLShader.h"

Asura::Ref<Asura::Shader> Asura::Shader::createShader(const std::string &filePath) {
    switch (Asura::RendererAPI::getAPI()) {
        case RendererAPI::API::None:
            ASURA_CORE_ASSERT(false, "RendererAPI::API::None is currently not supported");
            return nullptr;
        case RendererAPI::API::OpenGL:
            return createRef<OpenGLShader>(filePath);
    }
    return nullptr;
}

Asura::Ref<Asura::Shader> Asura::Shader::createShader(const std::string& name, const std::string &vertexSrc, const std::string &fragmentSrc) {
    switch (Asura::RendererAPI::getAPI()) {
        case RendererAPI::API::None:
            ASURA_CORE_ASSERT(false, "RendererAPI::API::None is currently not supported");
            return nullptr;
        case RendererAPI::API::OpenGL:
            return createRef<OpenGLShader>(name, vertexSrc, fragmentSrc);
    }
    return nullptr;
}




void Asura::ShaderLibrary::addShader(Asura::Ref<Asura::Shader> &shader) {
    ASURA_CORE_ASSERT(!exists(shader->getName()), "Shader already exists");
    m_shaders[shader->getName()] = shader;
}

void Asura::ShaderLibrary::addShader(const std::string &name, Asura::Ref<Asura::Shader> &shader) {
    ASURA_CORE_ASSERT(!exists(name), "Shader already exists");
    m_shaders[name] = shader;
}

void Asura::ShaderLibrary::addShader(const std::string &name, const std::string &filePath) {
    ASURA_CORE_ASSERT(!exists(name), "Shader already exists");
    auto shader = Asura::Shader::createShader(filePath);
    addShader(name, shader);
}

Asura::Ref<Asura::Shader> Asura::ShaderLibrary::getShader(const std::string &name) {
    return Asura::Ref<Asura::Shader>();
}

bool Asura::ShaderLibrary::exists(const std::string &name) const {
    return false;
}

Asura::Ref<Asura::Shader> Asura::ShaderLibrary::loadShader(const std::string &filePath) {
    auto shader = Shader::createShader(filePath);
    addShader(shader->getName(), shader);
    return shader;
}

Asura::Ref<Asura::Shader> Asura::ShaderLibrary::loadShader(const std::string &name, const std::string &vertexSrc,
                                                           const std::string &fragmentSrc) {
    auto shader = Shader::createShader(name, vertexSrc, fragmentSrc);
    addShader(name, shader);
    return shader;
}

