// @author: lx
// @time: 2022/11/2
// @brief: 

#ifndef GAME_OPENGLUNIFORMBUFFER_H
#define GAME_OPENGLUNIFORMBUFFER_H

#include "Core/Core.h"
#include "Renderer/UniformBuffer.h"


namespace Asura {
    class OpenGLUniformBuffer : public UniformBuffer {
    public:
        OpenGLUniformBuffer(uint32_t size, uint32_t binding);
        ~OpenGLUniformBuffer() override;

        void setData(const void *data, uint32_t size, uint32_t offset = 0) override;

    private:
        unsigned m_rendererID = 0;
    };
}


#endif //GAME_OPENGLUNIFORMBUFFER_H
