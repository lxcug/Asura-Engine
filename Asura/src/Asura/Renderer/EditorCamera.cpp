#include "EditorCamera.h"
#include "Core/Input.h"
#include "Events/MouseEvent.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/quaternion.hpp"

Asura::EditorCamera::EditorCamera(float fov, float aspect, float near, float far): m_FOV(fov), m_aspectRatio(aspect),
m_nearClip(near), m_farClip(far) {
    updateView();
}

void Asura::EditorCamera::onUpdate(Asura::Timestep timestep) {
    using namespace Asura;
    if(Input::isKeyPressed(Key::LeftAlt)) {
        const glm::vec2& mousePos = Input::getMousePosition();
        glm::vec2 delta = (mousePos - m_initialMousePosition) * 0.003f;
        m_initialMousePosition = mousePos;

        if(Input::isMouseButtonPressed(Mouse::ButtonMiddle)) {
            mousePan(delta);
        }
        if(Input::isMouseButtonPressed(Mouse::ButtonLeft)) {
            mouseRotate(delta);
        }
        if(Input::isMouseButtonPressed(Mouse::ButtonRight)) {
            mouseZoom(delta.y);
        }
    }
    updateView();
}

void Asura::EditorCamera::updateProjection() {
    m_aspectRatio = m_viewportWidth / m_viewportHeight;
    m_projection = glm::perspective(glm::radians(m_FOV), m_aspectRatio, m_nearClip, m_farClip);
}

void Asura::EditorCamera::updateView() {
    m_position = calculatePosition();
    glm::quat orientation = getOrientation();
    m_viewMatrix = glm::translate(glm::mat4(1.0f), m_position) * glm::toMat4(orientation);
    m_viewMatrix = glm::inverse(m_viewMatrix);
}

void Asura::EditorCamera::onEvent(Asura::Event &event) {
    EventDispatcher dispatcher(event);
    dispatcher.dispatch<MouseScrolledEvent>(BIND_EVENT_FUN(EditorCamera::onMouseScroll));
}

glm::vec3 Asura::EditorCamera::getUpDirection() const {
    return glm::rotate(getOrientation(), glm::vec3(0.f, 1.f, 0.f));
}

glm::vec3 Asura::EditorCamera::getRightDirection() const {
    return glm::rotate(getOrientation(), glm::vec3(1.f, 0.f, 0.f));
}

glm::vec3 Asura::EditorCamera::getForwardDirection() const {
    return glm::rotate(getOrientation(), glm::vec3(0.f, 0.f, -1.f));
}

glm::quat Asura::EditorCamera::getOrientation() const {
    return glm::quat(glm::vec3(-m_pitch, -m_yaw, 0.f));
}

bool Asura::EditorCamera::onMouseScroll(Asura::MouseScrolledEvent &event) {
    float delta = event.getYOffset() * 0.1f;
    mouseZoom(delta);
    updateView();
    return false;
}

void Asura::EditorCamera::mousePan(const glm::vec2 &delta) {
    auto [xSpeed, ySpeed] = panSpeed();
    m_focalPoint += -getRightDirection() * delta.x * xSpeed * m_distance;
    m_focalPoint += getUpDirection() * delta.y * ySpeed * m_distance;
}

void Asura::EditorCamera::mouseRotate(const glm::vec2 &delta) {
    float yawSign = getUpDirection().y < 0 ? -1.0f : 1.0f;
    m_yaw += yawSign * delta.x * rotationSpeed();
    m_pitch += delta.y * rotationSpeed();
}

void Asura::EditorCamera::mouseZoom(float delta) {
    m_distance -= delta * zoomSpeed();
    if(m_distance < 1.0f) {
        m_focalPoint += getForwardDirection();
        m_distance = 1.0f;
    }
}

glm::vec3 Asura::EditorCamera::calculatePosition() const {
    return m_focalPoint - getForwardDirection() * m_distance;
}

std::pair<float, float> Asura::EditorCamera::panSpeed() const {
    float x = std::min(m_viewportWidth / 1000.0f, 2.4f); // max = 2.4f
    float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;
    float y = std::min(m_viewportHeight / 1000.0f, 2.4f); // max = 2.4f
    float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

    return { xFactor, yFactor };
}

float Asura::EditorCamera::rotationSpeed() const {
    return .8f;
}

float Asura::EditorCamera::zoomSpeed() const {
    float distance = m_distance * 0.2f;
    distance = std::max(distance, 0.0f);
    float speed = distance * distance;
    speed = std::min(speed, 100.0f);  // max speed = 100
    return speed;
}
