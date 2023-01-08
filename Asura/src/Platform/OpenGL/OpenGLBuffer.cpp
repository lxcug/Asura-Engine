#include "OpenGLBuffer.h"
#include "glad/glad.h"

// --------------------------------------Vertex Buffer--------------------------------------

Asura::OpenGLVertexBuffer::OpenGLVertexBuffer(unsigned int size) {
    ASURA_PROFILE_FUNCTION();
    glGenBuffers(1, &m_rendererID);
    glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);  // bind vertex buffer
    glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
}

Asura::OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, unsigned size) {  // NOLINT
    ASURA_PROFILE_FUNCTION();
    glGenBuffers(1, &m_rendererID);
    glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);  // bind vertex buffer
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

Asura::OpenGLVertexBuffer::~OpenGLVertexBuffer() {
    ASURA_PROFILE_FUNCTION();
    glDeleteBuffers(1, &m_rendererID);
}

void Asura::OpenGLVertexBuffer::bind() const {
    ASURA_PROFILE_FUNCTION();
    glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);  // bind OpenGL vertex buffer
}

void Asura::OpenGLVertexBuffer::unBind() const {
    ASURA_PROFILE_FUNCTION();
    glBindBuffer(GL_ARRAY_BUFFER, 0);  // unbind OpenGL vertex buffer
}


// --------------------------------------Index Buffer--------------------------------------

Asura::OpenGLIndexBuffer::OpenGLIndexBuffer(unsigned int *indices, unsigned int count): m_count(count) {
    ASURA_PROFILE_FUNCTION();
    glGenBuffers(1, &m_rendererID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID);  // bind index buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned), indices, GL_STATIC_DRAW);
}

Asura::OpenGLIndexBuffer::~OpenGLIndexBuffer() {
    ASURA_PROFILE_FUNCTION();
    glDeleteBuffers(1, &m_rendererID);
}

void Asura::OpenGLIndexBuffer::bind() const {
    ASURA_PROFILE_FUNCTION();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID);  // bind OpenGL index buffer
}

void Asura::OpenGLIndexBuffer::unBind() const {
    ASURA_PROFILE_FUNCTION();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);  // unbind OpenGL index buffer
}
