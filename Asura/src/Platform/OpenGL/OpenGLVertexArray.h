// @author: lx
// @time: 2022/10/15
// @brief: 

#pragma once
#ifndef GAME_OPENGLVERTEXARRAY_H
#define GAME_OPENGLVERTEXARRAY_H

#include "Renderer/VertexArray.h"

namespace Asura {

    class ASURA_API OpenGLVertexArray : public VertexArray
    {
    public:
        OpenGLVertexArray();
        virtual ~OpenGLVertexArray();

        void bind() const override;
        void unbind() const override;

        void addVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) override;
        void setIndexBuffer(const Ref<IndexBuffer>& indexBuffer) override;

        [[nodiscard]] const std::vector<Ref<VertexBuffer>>& getVertexBuffers() const override {
            return m_vertexBuffers;
        }
        [[nodiscard]] const Ref<IndexBuffer>& getIndexBuffer() const override {
            return m_indexBuffer;
        }
    private:
        unsigned m_rendererID;
        unsigned m_vertexBufferIndex = 0;
        std::vector<Ref<VertexBuffer>> m_vertexBuffers;
        Ref<IndexBuffer> m_indexBuffer;
    };
}


#endif //GAME_OPENGLVERTEXARRAY_H
