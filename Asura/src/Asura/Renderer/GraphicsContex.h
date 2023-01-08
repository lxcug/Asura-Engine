// @author: lx
// @time: 2022/10/15
// @brief: 

#pragma once
#ifndef GAME_GRAPHICSCONTEX_H
#define GAME_GRAPHICSCONTEX_H

#include "Core/Core.h"


namespace Asura {
    class GraphicsContext {
    public:
        virtual ~GraphicsContext() {}

        virtual void init() = 0;
        virtual void swapBuffers() = 0;

        static Scope<GraphicsContext> createGraphicsContext(void* window);
    };
}


#endif //GAME_GRAPHICSCONTEX_H
