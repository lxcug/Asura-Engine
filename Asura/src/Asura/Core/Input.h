// @author: lx
// @time: 2022/10/14
// @brief: 

#pragma once
#ifndef GAME_INPUT_H
#define GAME_INPUT_H

#include "Core.h"
#include <iostream>
#include "Core/KeyCodes.h"
#include "Core/MouseCodes.h"
#include "glm/glm.hpp"


namespace Asura {
    /**
     * @brief provides input polling for Application.
     */
    class ASURA_API Input {
    public:
        static bool isKeyPressed(KeyCode keyCode);

        static bool isMouseButtonPressed(MouseCode mouseCode);

        static glm::vec2 getMousePosition();
        static float getMouseX();
        static float getMouseY();
    };
}


#endif //GAME_INPUT_H
