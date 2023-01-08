// @author: lx
// @time: 2022/10/16
// @brief: 

#pragma once
#ifndef GAME_ORTHOGRAPHICCAMERA_H
#define GAME_ORTHOGRAPHICCAMERA_H

#include "Asura/Core/Core.h"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>


namespace Asura {
    class ASURA_API OrthographicCamera {
    public:
        OrthographicCamera(float left, float right, float bottom, float top);

        [[nodiscard]] const glm::vec3& getPosition() const {
            return m_position;
        }
        [[nodiscard]] float getRotation() const {
            return m_rotation;
        }

        void setPosition(const glm::vec3& position) {
            ASURA_PROFILE_FUNCTION();
            m_position = position;
            recalculateViewMatrix();
        }
        void setRotation(float rotation) {
            ASURA_PROFILE_FUNCTION();
            m_rotation = rotation;
            recalculateViewMatrix();
        }
        void setProjection(float left, float right, float bottom, float top) {
            ASURA_PROFILE_FUNCTION();
            m_projectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
            m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
        }

        [[nodiscard]] const glm::mat4& getProjectionMatrix() const {
            return m_projectionMatrix;
        }
        [[nodiscard]] const glm::mat4& getViewMatrix() const {
            return m_viewMatrix;
        }
        [[nodiscard]] const glm::mat4& getViewProjectionMatrix() const {
            return m_viewProjectionMatrix;
        }


    private:
        glm::mat4 m_projectionMatrix;
        glm::mat4 m_viewMatrix;
        glm::mat4 m_viewProjectionMatrix;
        glm::vec3 m_position = { 0.0f, 0.0f, 0.0f };
        float m_rotation = 0.0f;

        void recalculateViewMatrix();
    };
}


#endif //GAME_ORTHOGRAPHICCAMERA_H
