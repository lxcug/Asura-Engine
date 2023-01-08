#include "OpenGLVertexArray.h"
#include "Glad/glad.h"
#include "Core/Log.h"


namespace Asura {
    static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
    {
        switch (type) {
            case ShaderDataType::Float:    return GL_FLOAT;
            case ShaderDataType::Float2:   return GL_FLOAT;
            case ShaderDataType::Float3:   return GL_FLOAT;
            case ShaderDataType::Float4:   return GL_FLOAT;
            case ShaderDataType::Mat3:     return GL_FLOAT;
            case ShaderDataType::Mat4:     return GL_FLOAT;
            case ShaderDataType::Int:      return GL_INT;
            case ShaderDataType::Int2:     return GL_INT;
            case ShaderDataType::Int3:     return GL_INT;
            case ShaderDataType::Int4:     return GL_INT;
            case ShaderDataType::Bool:     return GL_BOOL;
        }
        ASURA_CORE_ASSERT(false, "Unknown ShaderDataType!");
        return 0;
    }

    OpenGLVertexArray::OpenGLVertexArray() {
        ASURA_PROFILE_FUNCTION();
        glCreateVertexArrays(1, &m_rendererID);
    }

    OpenGLVertexArray::~OpenGLVertexArray() {
        ASURA_PROFILE_FUNCTION();
        glDeleteVertexArrays(1, &m_rendererID);
    }

    void OpenGLVertexArray::bind() const {
        ASURA_PROFILE_FUNCTION();
        glBindVertexArray(m_rendererID);
    }

    void OpenGLVertexArray::unbind() const {
        ASURA_PROFILE_FUNCTION();
        glBindVertexArray(0);
    }

    void OpenGLVertexArray::addVertexBuffer(const Asura::Ref<VertexBuffer> &vertexBuffer) {
        ASURA_PROFILE_FUNCTION();
        ASURA_CORE_ASSERT(vertexBuffer->getLayout().getElements().size(), "Vertex Buffer has no layout!");

        bind();
        vertexBuffer->bind();

        const auto& layout = vertexBuffer->getLayout();
        for (const auto& element : layout)
        {
            switch (element.Type)
            {
                case ShaderDataType::Float:
                case ShaderDataType::Float2:
                case ShaderDataType::Float3:
                case ShaderDataType::Float4: {
                    glEnableVertexAttribArray(m_vertexBufferIndex);
                    glVertexAttribPointer(m_vertexBufferIndex,
                                          element.getComponentCount(),
                                          ShaderDataTypeToOpenGLBaseType(element.Type),
                                          element.Normalized ? GL_TRUE : GL_FALSE,
                                          layout.getStride(),
                                          (const void*)element.Offset);
                    m_vertexBufferIndex++;
                    break;
                }
                case ShaderDataType::Int:
                case ShaderDataType::Int2:
                case ShaderDataType::Int3:
                case ShaderDataType::Int4:
                case ShaderDataType::Bool: {
                    glEnableVertexAttribArray(m_vertexBufferIndex);
                    glVertexAttribIPointer(m_vertexBufferIndex,
                                          (GLint)element.getComponentCount(),
                                          ShaderDataTypeToOpenGLBaseType(element.Type),
                                          (GLsizei)layout.getStride(),
                                          (const void*)element.Offset);
                    m_vertexBufferIndex++;
                    break;
                }
                case ShaderDataType::Mat3:
                case ShaderDataType::Mat4: {
                    uint8_t count = element.getComponentCount();
                    for (uint8_t i = 0; i < count; i++)
                    {
                        glEnableVertexAttribArray(m_vertexBufferIndex);
                        glVertexAttribPointer(m_vertexBufferIndex,
                                              count,
                                              ShaderDataTypeToOpenGLBaseType(element.Type),
                                              element.Normalized ? GL_TRUE : GL_FALSE,
                                              (GLsizei)layout.getStride(),
                                              (const void*)(element.Offset + sizeof(float) * count * i));
                        glVertexAttribDivisor(m_vertexBufferIndex, 1);
                        m_vertexBufferIndex++;
                    }
                    break;
                }
                default:
                    ASURA_CORE_ASSERT(false, "Unknown ShaderDataType!");
            }
        }
        m_vertexBuffers.push_back(vertexBuffer);
    }

    void OpenGLVertexArray::setIndexBuffer(const Asura::Ref<IndexBuffer> &indexBuffer) {
        ASURA_PROFILE_FUNCTION();
        bind();
        indexBuffer->bind();
        m_indexBuffer = indexBuffer;
    }
}

