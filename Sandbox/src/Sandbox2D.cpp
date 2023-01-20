#include "Sandbox2D.h"
#include "glm/gtc/type_ptr.hpp"
#include "Renderer/RenderCommand.h"
#include "imgui/imgui.h"

#include "Core/Application.h"


Sandbox2D::Sandbox2D(): Asura::Layer("Sandbox2D layer") {
    m_cameraController =
            std::make_shared<Asura::OrthographicCameraController>(1280.f/720.f, true);
    m_texture = Asura::Texture2D::createTexture2D("E:/Asura/Asura-Editor/assets/textures/dog.png");
}

void Sandbox2D::onAttach() {
    Layer::onAttach();
}

void Sandbox2D::onDetach() {
    Layer::onDetach();
}

void Sandbox2D::onUpdate(Asura::Timestep timestep) {
    Layer::onUpdate(timestep);
    using namespace Asura;

    Asura::Renderer2D::resetStats();
    ASURA_PROFILE_FUNCTION();
    {
        ASURA_PROFILE_SCOPE("Renderer prep");
        RenderCommand::clear();
        RenderCommand::setClearColor({ 0.3f, 0.4f, 0.5f, .3f});
    }

    m_cameraController->onUpdate(timestep);

    {
        ASURA_PROFILE_SCOPE("Renderer draw");

        static float rotation = 0.f;
        rotation += timestep * 50.f;
        ASURA_INFO("rotation {}", rotation);

        Renderer2D::beginScene(m_cameraController->getCamera());

        static glm::vec4 color = {0.6f, .3f, .5f, .3f};
        for (float y = -5.0f; y < 5.0f; y += 0.5f)
        {
            for (float x = -5.0f; x < 5.0f; x += 0.5f)
            {
                Asura::Renderer2D::drawQuad({ x, y }, { 0.49f, 0.49f }, color);
            }
        }

        for(int i = 0; i < 90; i += 90/5) {
            Renderer2D::drawRotateQuad({0.f, 0.f}, {1.f, 1.f}, glm::radians(i+rotation), m_color);
        }

        Renderer2D::endScene();
    }
}

void Sandbox2D::onEvent(Asura::Event &event) {
    Layer::onEvent(event);
    m_cameraController->onEvent(event);
}

void Sandbox2D::onImGuiRender() {
    ASURA_PROFILE_FUNCTION();
    ImGui::Begin("Settings");

    auto stats = Asura::Renderer2D::getStats();
    ImGui::Text("Renderer2D Stats:");
    ImGui::Text("Draw Calls: %d", stats.DrawCalls);
    ImGui::Text("Quads: %d", stats.QuadCount);
    ImGui::Text("Vertices: %d", stats.getTotalVertexCount());
    ImGui::Text("Indices: %d", stats.getTotalIndexCount());
    ImGui::ColorEdit4("Color", glm::value_ptr(m_color));
    ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
    ImGui::End();
}
