#include "OrthographicCamera.h"


Asura::OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top):
m_projectionMatrix(glm::ortho(left, right, bottom, top, -1.f, 1.f)), m_viewMatrix(1.f)
{
    ASURA_PROFILE_FUNCTION();
    m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
}

void Asura::OrthographicCamera::recalculateViewMatrix() {
    ASURA_PROFILE_FUNCTION();
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_position) *
                          glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation),
                                      glm::vec3(0, 0, 1));

    m_viewMatrix = glm::inverse(transform);
    m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
}

