// @author: lx
// @time: 2022/10/27
// @brief: 

#pragma once
#ifndef GAME_SCENECAMERA_H
#define GAME_SCENECAMERA_H

#include "Core/Core.h"
#include "Renderer/Camera.h"
#include "Core/Log.h"


namespace Asura {
    class SceneCamera : public Camera {
    public:
        SceneCamera();
        virtual ~SceneCamera() = default;

        void setViewportSize(uint32_t width, uint32_t height);

        [[nodiscard]] float getAspectRatio() {
            return m_aspectRatio;
        }

        /*
         * orthographic
         */
        void setOrthographic(float size, float nearClip, float farClip);

        float getOrthographicSize() const {
            return m_orthographicSize;
        }
        void setOrthographicSize(float size) {
            m_orthographicSize = size;
            recalculateProjection();
        }

        float getOrthographicNearClip() const {
            return m_orthographicNear;
        }
        void setOrthographicNearClip(float nearClip) {
            m_orthographicNear = nearClip;
            recalculateProjection();
        }
        float getOrthographicFarClip() const {
            return m_orthographicFar;
        }
        void setOrthographicFarClip(float farClip) {
            m_orthographicFar = farClip;
            recalculateProjection();
        }


        /*
         * perspective
         */
        void setPerspective(float verticalFOV, float nearClip, float farClip);

        float getPerspectiveVerticalFOV() const {
            return m_perspectiveFOV;
        }
        void setPerspectiveVerticalFOV(float verticalFOV) {
            m_perspectiveFOV = verticalFOV;
            recalculateProjection();
        }

        float getPerspectiveNearClip() const {
            return m_perspectiveNear;
        }
        void setPerspectiveNearClip(float nearClip) {
            m_perspectiveNear = nearClip;
            recalculateProjection();
        }
        float getPerspectiveFarClip() const {
            return m_perspectiveFar;
        }
        void setPerspectiveFarClip(float farClip) {
            m_perspectiveFar = farClip;
            recalculateProjection();
        }

        enum class ProjectionType {
            Perspective = 0x0,
            Orthographic
        };
        ProjectionType getProjectionType() const {
            return m_projectionType;
        }
        void setProjectionType(ProjectionType type) {
            m_projectionType = type;
            recalculateProjection();
        }

    private:
        float m_orthographicSize = 10.f;
        float m_orthographicNear = -1.f, m_orthographicFar = 1.f;
        float m_aspectRatio = 1.778f;

        ProjectionType m_projectionType = ProjectionType::Perspective;

        float m_perspectiveFOV = 45.f;
        float m_perspectiveNear = .1f, m_perspectiveFar = 1000.f;

        void recalculateProjection();
    };
}


#endif //GAME_SCENECAMERA_H
