// @author: lx
// @time: 2022/11/1
// @brief: 

#ifndef GAME_EDITORCAMERA_H
#define GAME_EDITORCAMERA_H

#include "Core/Core.h"
#include "glm/glm.hpp"
#include "Renderer/Camera.h"
#include "Core/Timestep.h"
#include "Events/Event.h"
#include "Events/MouseEvent.h"


namespace Asura {
    class EditorCamera : public Camera {
    public:
        EditorCamera() = default;
        EditorCamera(float fov, float aspect, float near, float far);

        void onUpdate(Timestep timestep);
        void onEvent(Event& event);

        [[nodiscard]] float getDistance() const {
            return m_distance;
        }
        void setDistance(float dis) {
            m_distance = dis;
        }

        void setViewportSize(float width, float height) {
            m_viewportWidth = width;
            m_viewportHeight = height;
            updateProjection();
        }

        [[nodiscard]] const glm::mat4& getViewMatrix() const {
            return m_viewMatrix;
        }
        [[nodiscard]] glm::mat4 getViewProjection() const {
            return m_projection * m_viewMatrix;
        };

        glm::vec3 getUpDirection() const;
        glm::vec3 getRightDirection() const;
        glm::vec3 getForwardDirection() const;
        const glm::vec3& getPosition() const {
            return m_position;
        }
        glm::quat getOrientation() const;

        float getPitch() const {
            return m_pitch;
        }
        float getYaw() const {
            return m_yaw;
        }

    private:
        void updateProjection();
        void updateView();

        bool onMouseScroll(MouseScrolledEvent& event);

        void mousePan(const glm::vec2& delta);
        void mouseRotate(const glm::vec2& delta);
        void mouseZoom(float delta);

        glm::vec3 calculatePosition() const;

        std::pair<float, float> panSpeed() const;
        float rotationSpeed() const;
        float zoomSpeed() const;

    private:
        float m_FOV = 45.f, m_aspectRatio = 1.778f;
        float m_nearClip = .1f, m_farClip = 1000.f;

        glm::mat4 m_viewMatrix;
        glm::vec3 m_position = {0.f, 0.f, 0.f};
        glm::vec3 m_focalPoint = {0.f, 0.f, 0.f};
        glm::vec2 m_initialMousePosition = {0.f, 0.f};

        float m_distance = 10.f;
        float m_pitch = 0.f, m_yaw = 0.f;
        float m_viewportWidth = 1280.f, m_viewportHeight = 720.f;
    };
}


#endif //GAME_EDITORCAMERA_H
