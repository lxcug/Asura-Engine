// @author: lx
// @time: 2022/10/16
// @brief: 

#pragma once
#ifndef GAME_OPENGLRENDERERAPI_H
#define GAME_OPENGLRENDERERAPI_H

#include "Renderer/RendererAPI.h"
#include "Glad/glad.h"


namespace Asura {
    class ASURA_API OpenGLRendererAPI : public RendererAPI {
    public:
        void init() override {
            ASURA_PROFILE_FUNCTION();
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            glEnable(GL_DEPTH_TEST);
            glEnable(GL_LINE_SMOOTH);
        }

        void setClearColor(const glm::vec4 &color) override {
            glClearColor(color.r, color.g, color.b, color.a);
        }

        void drawIndexed(const Ref<VertexArray> &vertexArray, uint32_t indexCount = 0) override {
            vertexArray->bind();
            unsigned count = indexCount ? indexCount : vertexArray->getIndexBuffer()->getCount();
            glDrawElements(GL_TRIANGLES, (GLsizei)count, GL_UNSIGNED_INT, nullptr);
        }

        void drawLines(const Ref<Asura::VertexArray> &vertexArray, uint32_t vertexCount) override {
            vertexArray->bind();
            glDrawArrays(GL_LINES, 0, (int)vertexCount);
        }
        void setLineWidth(float width) override {
            glLineWidth(width);
        }

        void clear() override {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }

        void setViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height) override {
            glViewport((GLint)x, (GLint)y, (GLint)width, (GLint)height);
        }
    };
}

#endif //GAME_OPENGLRENDERERAPI_H
