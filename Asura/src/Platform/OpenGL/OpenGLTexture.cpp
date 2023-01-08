#include "OpenGLTexture.h"
#include "Glad/glad.h"
#include "Core/Log.h"


Asura::OpenGLTexture2D::OpenGLTexture2D(std::string path): m_path(std::move(path)) {
    ASURA_PROFILE_FUNCTION();
    int width, height, channels;
    stbi_set_flip_vertically_on_load(1);

    stbi_uc* data = nullptr;
    {
        ASURA_PROFILE_SCOPE("stbi_load - OpenGLTexture2D::OpenGLTexture2D(const std::string&)");
        data = stbi_load(m_path.c_str(), &width, &height, &channels, 0);
    }
    if(data) {
        m_isLoaded = true;
        m_width = width;
        m_height = height;
        if(channels == 4) {
            m_internalFormat = GL_RGBA8;
            m_dataFormat = GL_RGBA;
        }
        else if(channels == 3) {
            m_internalFormat = GL_RGB8;
            m_dataFormat = GL_RGB;
        }

        ASURA_CORE_ASSERT(m_internalFormat & m_dataFormat, "Format not supported!");

        glCreateTextures(GL_TEXTURE_2D, 1, &m_rendererID);
        glTextureStorage2D(m_rendererID, 1, m_internalFormat, m_width, m_height);

        glTextureParameteri(m_rendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_rendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);


        glTextureSubImage2D(m_rendererID, 0, 0, 0, m_width, m_height, m_dataFormat, GL_UNSIGNED_BYTE, data);

        stbi_image_free(data);
    }
}

Asura::OpenGLTexture2D::OpenGLTexture2D(unsigned int width, unsigned int height): m_width(width), m_height(height) {
    ASURA_PROFILE_FUNCTION();

    m_isLoaded = true;

    m_internalFormat = GL_RGBA8;
    m_dataFormat = GL_RGBA;

    glCreateTextures(GL_TEXTURE_2D, 1, &m_rendererID);
    glTextureStorage2D(m_rendererID, 1, m_internalFormat, m_width, m_height);

    glTextureParameteri(m_rendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_rendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

Asura::OpenGLTexture2D::~OpenGLTexture2D() {
    ASURA_PROFILE_FUNCTION();
    glDeleteTextures(1, &m_rendererID);
}

void Asura::OpenGLTexture2D::bind(unsigned int slot) {
    ASURA_PROFILE_FUNCTION();
    glBindTextureUnit(slot, m_rendererID);
}

void Asura::OpenGLTexture2D::SetData(void *data, uint32_t size) {
    ASURA_PROFILE_FUNCTION();
    uint32_t bpp = m_dataFormat == GL_RGBA ? 4 : 3;
    ASURA_CORE_ASSERT(size == m_width * m_height * bpp, "Data must be entire texture!");
    glTexImage2D(GL_TEXTURE_2D, 0, m_internalFormat, m_width, m_height, 0, m_dataFormat, GL_UNSIGNED_BYTE, data);
}

void Asura::OpenGLTexture2D::unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
}
