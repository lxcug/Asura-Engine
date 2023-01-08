// @author: lx
// @time: 2022/10/16
// @brief: 

#pragma once
#ifndef GAME_RENDERCOMMAND_H
#define GAME_RENDERCOMMAND_H

#include "Core/Core.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Asura {
    class ASURA_API RenderCommand {
    public:
        static void init() {
            s_rendererAPI->init();
        }

        static void setClearColor(const glm::vec4& color) {
            s_rendererAPI->setClearColor(color);
        }

        static void clear() {
            s_rendererAPI->clear();
        }

        static void setViewport(unsigned x, unsigned y, unsigned width, unsigned height) {
            s_rendererAPI->setViewport(x, y, width, height);
        }

        static void drawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t indexCount = 0) {
            s_rendererAPI->drawIndexed(vertexArray, indexCount);
        }

        static void drawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount) {
            s_rendererAPI->drawLines(vertexArray, vertexCount);
        }

        static void setLineWidth(float width) {
            s_rendererAPI->setLineWidth(width);
        }

    private:
        static Scope<RendererAPI> s_rendererAPI;
    };
}

#endif //GAME_RENDERCOMMAND_H
