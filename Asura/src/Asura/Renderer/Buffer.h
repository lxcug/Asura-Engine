// @author: lx
// @time: 2022/10/15
// @brief: 

#pragma once
#ifndef GAME_BUFFER_H
#define GAME_BUFFER_H

#include "Asura/Core/Core.h"
#include "Asura/Core/Log.h"

namespace Asura {
    enum class ShaderDataType {
        None = 0,
        Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool,
    };

    static unsigned ShaderDataTypeSize(ShaderDataType type) {
        switch (type) {
            case ShaderDataType::Float:
                return 4;
            case ShaderDataType::Float2:
                return 8;
            case ShaderDataType::Float3:
                return 12;
            case ShaderDataType::Float4:
                return 16;
            case ShaderDataType::Mat3:
                return 36;
            case ShaderDataType::Mat4:
                return 64;
            case ShaderDataType::Int:
                return 4;
            case ShaderDataType::Int2:
                return 8;
            case ShaderDataType::Int3:
                return 12;
            case ShaderDataType::Int4:
                return 16;
            case ShaderDataType::Bool:
                return 1;
            case ShaderDataType::None:
                break;
        }
        ASURA_CORE_ASSERT(false, "Unknown ShaderDataType");
        return 0;
    }

    struct BufferElement {
        std::string Name;
        ShaderDataType Type;
        unsigned Size;
        size_t Offset = 0;
        bool Normalized;

        BufferElement() = default;
        BufferElement(ShaderDataType type, std::string name, bool normalized=false): Type(type),
        Name(std::move(name)), Size(ShaderDataTypeSize(type)), Normalized(normalized) {}

        unsigned getComponentCount() const {
            switch (Type) {
                case ShaderDataType::Float:
                    return 1;
                case ShaderDataType::Float2:
                    return 2;
                case ShaderDataType::Float3:
                    return 3;
                case ShaderDataType::Float4:
                    return 4;
                case ShaderDataType::Mat3:
                    return 9;
                case ShaderDataType::Mat4:
                    return 16;
                case ShaderDataType::Int:
                    return 1;
                case ShaderDataType::Int2:
                    return 2;
                case ShaderDataType::Int3:
                    return 3;
                case ShaderDataType::Int4:
                    return 4;
                case ShaderDataType::Bool:
                    return 1;
            }
            ASURA_CORE_ASSERT(false, "Unknown ShaderDataType");
            return 0;
        }
    };


    class ASURA_API BufferLayout {
    public:
        BufferLayout() = default;
        BufferLayout(const std::initializer_list<BufferElement>& elements): m_elements(elements) {
            calculateOffsetsAndStride();
        }

        inline unsigned getStride() const {
            return m_stride;
        }

        inline const std::vector<BufferElement>& getElements() const {
            return m_elements;
        }

        std::vector<BufferElement>::iterator begin() {
            return m_elements.begin();
        }
        std::vector<BufferElement>::iterator end() {
            return m_elements.end();
        }
        [[nodiscard]] std::vector<BufferElement>::const_iterator begin() const {
            return m_elements.begin();
        }
        [[nodiscard]] std::vector<BufferElement>::const_iterator end() const {
            return m_elements.end();
        }

    private:
        std::vector<BufferElement> m_elements;
        unsigned m_stride = 0;

        void calculateOffsetsAndStride()
        {
            uint32_t offset = 0;
            m_stride = 0;
            for(auto& element : m_elements)
            {
                element.Offset = offset;
                offset += element.Size;
                m_stride += element.Size;
            }
        }
    };


    /**
     * @brief abstract of Vertex Buffer
     */
    class ASURA_API VertexBuffer {
    public:
        virtual ~VertexBuffer() = default;

        virtual void bind() const = 0;
        virtual void unBind() const = 0;

        virtual void setData(const void* data, unsigned size) = 0;

        virtual const BufferLayout& getLayout() const = 0;
        virtual void setLayout(const BufferLayout& layout) = 0;

        static Ref<VertexBuffer> createVertexBuffer(unsigned size);
        static Ref<VertexBuffer> createVertexBuffer(float* vertices, unsigned size);
    };


    /**
     * @brief abstract of Index Buffer
     */
    class ASURA_API IndexBuffer {
    public:
        virtual ~IndexBuffer() = default;

        virtual void bind() const = 0;
        virtual void unBind() const = 0;
        [[nodiscard]] virtual unsigned getCount() const = 0;

        static Ref<IndexBuffer> createIndexBuffer(unsigned* indices, unsigned count);
    };

}  // end namespace Asura




#endif //GAME_BUFFER_H
