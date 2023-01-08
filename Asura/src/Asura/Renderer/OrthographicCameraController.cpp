#include "Renderer/OrthographicCameraController.h"
#include "Core/KeyCodes.h"
#include "Core/MouseCodes.h"


Asura::OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation):
m_aspectRatio(aspectRatio), m_rotation(rotation),
m_camera(-m_aspectRatio*m_zoomLevel, m_aspectRatio*m_zoomLevel, -m_zoomLevel, m_zoomLevel)
{

}

void Asura::OrthographicCameraController::onUpdate(Asura::Timestep timestep) {
    ASURA_PROFILE_FUNCTION();
    using namespace Asura;

    // position
    if(Input::isKeyPressed(Key::W)) {
        m_cameraPosition.y += m_cameraTranslationSpeed * timestep;
    }
    else if(Input::isKeyPressed(Key::S)) {
        m_cameraPosition.y -= m_cameraTranslationSpeed * timestep;
    }
    if(Input::isKeyPressed(Key::A)) {
        m_cameraPosition.x -= m_cameraTranslationSpeed * timestep;
    }
    else if(Input::isKeyPressed(Key::D)) {
        m_cameraPosition.x += m_cameraTranslationSpeed * timestep;
    }

    // rotate
    if(m_rotation) {
        if(Input::isKeyPressed(Key::Q)) {
            m_cameraRotation -= m_cameraRotationSpeed * timestep;
        }
        else if(Input::isKeyPressed(Key::E)) {
            m_cameraRotation += m_cameraRotationSpeed * timestep;
        }
        m_camera.setRotation(m_cameraRotation);
    }
    m_camera.setPosition(m_cameraPosition);
    m_cameraTranslationSpeed = m_zoomLevel;
}

void Asura::OrthographicCameraController::onEvent(Event &event) {
    ASURA_PROFILE_FUNCTION();
    EventDispatcher dispatcher(event);
    dispatcher.dispatch<MouseScrolledEvent>(BIND_EVENT_FUN(OrthographicCameraController::onMouseScrolled));
    dispatcher.dispatch<WindowResizeEvent>(BIND_EVENT_FUN(OrthographicCameraController::onWindowResize));
}

bool Asura::OrthographicCameraController::onMouseScrolled(MouseScrolledEvent &event) {
    ASURA_PROFILE_FUNCTION();
    // zoom
    m_zoomLevel -= event.getYOffset() * 0.25f;
    m_zoomLevel = m_zoomLevel > 0.25f ? m_zoomLevel : 0.25f;
    m_camera.setProjection(-m_aspectRatio * m_zoomLevel, m_aspectRatio * m_zoomLevel,
                           -m_zoomLevel, m_zoomLevel);
    return false;
}

void Asura::OrthographicCameraController::onResize(float width, float height) {
    m_aspectRatio = width / height;
    m_camera.setProjection(-m_aspectRatio * m_zoomLevel, m_aspectRatio * m_zoomLevel,
                           -m_zoomLevel, m_zoomLevel);
}

bool Asura::OrthographicCameraController::onWindowResize(WindowResizeEvent &event) {
    ASURA_PROFILE_FUNCTION();
    onResize((float)event.getWidth(), (float)event.getHeight());
    return false;
}
