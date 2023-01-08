// @author: lx
// @time: 2022/10/27
// @brief: 

#pragma once
#ifndef GAME_CAMERA_H
#define GAME_CAMERA_H

#include "Core/Core.h"
#include "glm/glm.hpp"


namespace Asura {
    class Camera {
    public:
        Camera() = default;
        virtual ~Camera() = default;

        Camera(const glm::mat4& projection): m_projection(projection) {}

        const glm::mat4& getProjection() const {
            return m_projection;
        }

    protected:
        glm::mat4 m_projection = glm::mat4(1.f);
    };
}


#endif //GAME_CAMERA_H
