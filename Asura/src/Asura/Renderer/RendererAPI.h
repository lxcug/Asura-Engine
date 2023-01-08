// @author: lx
// @time: 2022/10/16
// @brief: 

#pragma once
#ifndef GAME_RENDERERAPI_H
#define GAME_RENDERERAPI_H

#include "Core/Core.h"
#include "glm/glm.hpp"
#include "Renderer/VertexArray.h"

namespace Asura {
    class RendererAPI {
    public:
        virtual ~RendererAPI() = default;

        enum class API {
            None = 0x0,
            OpenGL = 0x1,
        };

        virtual void init() = 0;

        virtual void setClearColor(const glm::vec4& color) = 0;

        virtual void clear() = 0;

        virtual void drawIndexed(const Ref<VertexArray>& vertexArray, unsigned indexCount) = 0;

        virtual void setViewport(unsigned x, unsigned y, unsigned width, unsigned height) = 0;

        virtual void drawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount) = 0;
        virtual void setLineWidth(float width) = 0;

        // see effective c++
        inline static API& getAPI() {
            static API instance = API::OpenGL;
            return instance;
        }

        static Scope<RendererAPI> createRendererAPI();
    };
}


#endif //GAME_RENDERERAPI_H
