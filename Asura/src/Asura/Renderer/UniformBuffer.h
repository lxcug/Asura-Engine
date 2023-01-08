// @author: lx
// @time: 2022/11/2
// @brief: 

#ifndef GAME_UNIFORMBUFFER_H
#define GAME_UNIFORMBUFFER_H

#include "Core/Core.h"


namespace Asura {
    class UniformBuffer {
    public:
        virtual ~UniformBuffer() {}

        virtual void setData(const void* data, uint32_t size, uint32_t offset = 0) = 0;

        static Ref<UniformBuffer> createUniformBuffer(uint32_t size, uint32_t binding);
    };
}


#endif //GAME_UNIFORMBUFFER_H
