// @author: lx
// @time: 2022/10/19
// @brief: 

#pragma once
#ifndef GAME_ORTHOGRAPHICCAMERACONTROLLER_H
#define GAME_ORTHOGRAPHICCAMERACONTROLLER_H

#include "Asura/Core/Core.h"
#include "Renderer/OrthographicCamera.h"
#include "Core/Timestep.h"
#include "Events/Event.h"
#include "Events/MouseEvent.h"
#include "Events/ApplicationEvent.h"
#include "Asura/Core/Input.h"
#include "Asura/Core/KeyCodes.h"


namespace Asura {
    class ASURA_API OrthographicCameraController {
    public:
        OrthographicCameraController(float aspectRatio, bool rotation=false);  // NOLINT

        void onUpdate(Timestep timestep);
        void onEvent(Event& event);

        void onResize(float width, float height);

        float getZoomLevel() const {
            return m_zoomLevel;
        }
        void setZoomLevel(float zoom) {
            m_zoomLevel = zoom;
        }

        OrthographicCamera& getCamera() {
            return m_camera;
        };
        [[nodiscard]] const OrthographicCamera& getCamera() const {
            return m_camera;
        };

    private:
        float m_aspectRatio;
        float m_zoomLevel = 1.f;
        bool m_rotation;
        glm::vec3 m_cameraPosition = {0.f, 0.f, 0.f};
        float m_cameraRotation = 0.f;
        float m_cameraTranslationSpeed = 5.f;
        float m_cameraRotationSpeed = 180.f;

        /**
         * @brief m_camera should behind m_aspectRatio and m_zoomLevel. cause in constructor
         * initialize list the initialize order of parameters is the order that they are defined.
         */
        Asura::OrthographicCamera m_camera;

        bool onMouseScrolled(MouseScrolledEvent& event);
        bool onWindowResize(WindowResizeEvent& event);
    };
}


#endif //GAME_ORTHOGRAPHICCAMERACONTROLLER_H
