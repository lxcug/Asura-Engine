#include "Renderer.h"
#include "Renderer/RenderCommand.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Renderer/Renderer2D.h"

Asura::Renderer::SceneData* Asura::Renderer::m_sceneData = new Asura::Renderer::SceneData;

void Asura::Renderer::init() {
    ASURA_PROFILE_FUNCTION();
    Asura::RenderCommand::init();
    Asura::Renderer2D::init();
}

void Asura::Renderer::shutDown() {
    Asura::Renderer2D::shutDown();
}

void Asura::Renderer::beginScene(Asura::OrthographicCamera &orthographicCamera) {
    ASURA_PROFILE_FUNCTION();
    m_sceneData->ViewProjectionMatrix = orthographicCamera.getViewProjectionMatrix();
}

void Asura::Renderer::endScene() {
    ASURA_PROFILE_FUNCTION();
}

void Asura::Renderer::submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray> &vertexArray,
                             const glm::mat4& transform) {
    ASURA_PROFILE_FUNCTION();
    shader->bind();
    std::dynamic_pointer_cast<OpenGLShader>(shader)->uploadUniformMat4("u_ViewProjection", m_sceneData->ViewProjectionMatrix);
    std::dynamic_pointer_cast<OpenGLShader>(shader)->uploadUniformMat4("u_Transform", transform);
    vertexArray->bind();
    Asura::RenderCommand::drawIndexed(vertexArray);
}

void Asura::Renderer::onWindowResize(unsigned int width, unsigned int height) {
    RenderCommand::setViewport(0, 0, width, height);
}
