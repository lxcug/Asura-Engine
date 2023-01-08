#include "Application.h"

#include <memory>
#include <filesystem>
#include "Renderer/Texture.h"
#include "Renderer/OrthographicCameraController.h"


Asura::Application* Asura::Application::s_instance = nullptr;

Asura::Application::Application(const Asura::ApplicationSpecification &specification) {
    ASURA_PROFILE_FUNCTION();
    ASURA_CORE_ASSERT(!s_instance, "Application already exist");
    s_instance = this;

    if (!m_specification.WorkingDirectory.empty())
        std::filesystem::current_path(m_specification.WorkingDirectory);

    m_window = Window::createWindow(WindowProps(m_specification.Name));

    m_window->setEventCallback(BIND_EVENT_FUN(Asura::Application::onEvent));
    Renderer::init();
}

Asura::Application::~Application() {
    ASURA_PROFILE_FUNCTION();
    Asura::Renderer::shutDown();
}

void Asura::Application::pushLayer(Asura::Layer *layer) {
    ASURA_PROFILE_FUNCTION();
    m_layerStack.pushLayer(layer);
}

void Asura::Application::pushOverlay(Asura::Layer *overlay) {
    ASURA_PROFILE_FUNCTION();
    m_layerStack.pushOverlay(overlay);
}
