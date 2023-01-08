#include "SceneCamera.h"
#include "glm/gtc/matrix_transform.hpp"


Asura::SceneCamera::SceneCamera() {
    recalculateProjection();
}

void Asura::SceneCamera::setOrthographic(float size, float nearClip, float farClip) {
    m_projectionType = ProjectionType::Orthographic;
    m_orthographicSize = size;
    m_orthographicNear = nearClip;
    m_orthographicFar = farClip;
    recalculateProjection();
}

void Asura::SceneCamera::setPerspective(float verticalFOV, float nearClip, float farClip) {
    m_projectionType = ProjectionType::Perspective;
    m_perspectiveFOV = verticalFOV;
    m_perspectiveNear = nearClip;
    m_perspectiveFar = farClip;
    recalculateProjection();
}

void Asura::SceneCamera::setViewportSize(uint32_t width, uint32_t height) {
    m_aspectRatio = (float)width / (float)height;
    recalculateProjection();
}

void Asura::SceneCamera::recalculateProjection() {
    if(m_projectionType == ProjectionType::Perspective) {
        m_projection = glm::perspective(glm::radians(m_perspectiveFOV), m_aspectRatio,
                                        m_perspectiveNear, m_perspectiveFar);
    }
    else {
        float orthoLeft = -m_orthographicSize * m_aspectRatio * .5f;
        float orthoRight = m_orthographicSize * m_aspectRatio * .5f;
        float orthoBottom = -m_orthographicSize * .5f;
        float orthoTop = m_orthographicSize * .5f;
        m_projection = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop,
                                  m_orthographicNear, m_orthographicFar);
    }
}


