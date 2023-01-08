#include "OpenGLFrameBuffer.h"

#include <utility>
#include "Glad/glad.h"
#include "Core/Log.h"


namespace Asura {
    static const uint32_t s_MaxFramebufferSize = 8192;

    namespace Utils {
        static GLenum TextureTarget(bool multisampled) {
            return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
        }

        static void CreateTextures(bool multisampled, uint32_t* outID, uint32_t count) {
            glCreateTextures(TextureTarget(multisampled), count, outID);
        }

        static void BindTexture(bool multisampled, uint32_t id) {
            glBindTexture(TextureTarget(multisampled), id);
        }

        static void AttachColorTexture(uint32_t id, int samples, GLenum internalFormat, GLenum format, uint32_t width, uint32_t height, int index) {
            bool multisampled = samples > 1;
            if (multisampled) {
                glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_FALSE);
            }
            else {
                glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            }
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(multisampled), id, 0);
        }

        static void AttachDepthTexture(uint32_t id, int samples, GLenum format, GLenum attachmentType, uint32_t width, uint32_t height) {
            bool multisampled = samples > 1;
            if (multisampled) {
                glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
            }
            else {
                glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            }
            glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, TextureTarget(multisampled), id, 0);
        }

        static bool IsDepthFormat(FrameBufferTextureFormat format) {
            switch (format) {
                case FrameBufferTextureFormat::DEPTH24STENCIL8:  return true;
            }
            return false;
        }

        static GLenum AsuraFBTextureFormatToGL(FrameBufferTextureFormat format) {
            switch (format) {
                case FrameBufferTextureFormat::RGBA8:       return GL_RGBA8;
                case FrameBufferTextureFormat::RED_INTEGER: return GL_RED_INTEGER;
            }
            ASURA_CORE_ASSERT(false, "");
            return 0;
        }
    }
}


Asura::OpenGLFrameBuffer::OpenGLFrameBuffer(Asura::FrameBufferSpecification spec): m_specification(std::move(spec)) {
    for (auto specification : m_specification.Attachments.Attachments) {
        if (!Utils::IsDepthFormat(specification.TextureFormat))
            m_colorAttachmentSpecifications.emplace_back(specification);
        else
            m_depthAttachmentSpecification = specification;
    }
    invalidate();
}

void Asura::OpenGLFrameBuffer::invalidate() {
    using namespace Asura;

    if(m_rendererID) {
        glDeleteFramebuffers(1, &m_rendererID);
        glDeleteTextures(m_colorAttachments.size(), m_colorAttachments.data());
        glDeleteTextures(1, &m_depthAttachment);

        m_colorAttachments.clear();
        m_depthAttachment = 0;
    }

    glCreateFramebuffers(1, &m_rendererID);
    glBindFramebuffer(GL_FRAMEBUFFER, m_rendererID);

    bool multisample = m_specification.Samples > 1;

    // Attachments
    if(m_colorAttachmentSpecifications.size()) {
        m_colorAttachments.resize(m_colorAttachmentSpecifications.size());
        Utils::CreateTextures(multisample, m_colorAttachments.data(), m_colorAttachments.size());

        for(size_t i = 0; i < m_colorAttachments.size(); i++) {
            Utils::BindTexture(multisample, m_colorAttachments[i]);
            switch (m_colorAttachmentSpecifications[i].TextureFormat) {
                case FrameBufferTextureFormat::RGBA8:
                    Utils::AttachColorTexture(m_colorAttachments[i], m_specification.Samples,
                                              GL_RGBA8, GL_RGBA, m_specification.Width,
                                              m_specification.Height, i);
                    break;
                case FrameBufferTextureFormat::RED_INTEGER:
                    Utils::AttachColorTexture(m_colorAttachments[i], m_specification.Samples,
                                              GL_R32I, GL_RED_INTEGER, m_specification.Width,
                                              m_specification.Height, i);
                    break;
            }
        }
    }

    if(m_depthAttachmentSpecification.TextureFormat != FrameBufferTextureFormat::None) {
        Utils::CreateTextures(multisample, &m_depthAttachment, 1);
        Utils::BindTexture(multisample, m_depthAttachment);
        switch (m_depthAttachmentSpecification.TextureFormat) {
            case FrameBufferTextureFormat::DEPTH24STENCIL8:
                Utils::AttachDepthTexture(m_depthAttachment, m_specification.Samples,
                                          GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT,
                                          m_specification.Width, m_specification.Height);
                break;
        }
    }

    if(m_colorAttachments.size() > 1) {
        ASURA_CORE_ASSERT(m_colorAttachments.size() <= 4, "");
        GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
        glDrawBuffers(m_colorAttachments.size(), buffers);
    }
    else if(m_colorAttachments.empty()) {
        // Only depth-pass
        glDrawBuffer(GL_NONE);
    }
    ASURA_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Asura::OpenGLFrameBuffer::~OpenGLFrameBuffer() {
    glDeleteFramebuffers(1, &m_rendererID);
    glDeleteTextures(m_colorAttachments.size(), m_colorAttachments.data());
    glDeleteTextures(1, &m_depthAttachment);
}

void Asura::OpenGLFrameBuffer::bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, m_rendererID);
    glViewport(0, 0, m_specification.Width, m_specification.Height);
}

void Asura::OpenGLFrameBuffer::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Asura::OpenGLFrameBuffer::resize(unsigned int width, unsigned int height) {
    m_specification.Width = width;
    m_specification.Height = height;
    invalidate();
}

int Asura::OpenGLFrameBuffer::readPixel(uint32_t attachmentIndex, int x, int y) {
    ASURA_CORE_ASSERT(attachmentIndex < m_colorAttachments.size(), "");

    glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
    int pixelData;
    glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
    return pixelData;
}

void Asura::OpenGLFrameBuffer::clearAttachment(uint32_t attachmentIndex, int value) {
    ASURA_CORE_ASSERT(attachmentIndex < m_colorAttachments.size(), "");

    auto& spec = m_colorAttachmentSpecifications[attachmentIndex];
    glClearTexImage(m_colorAttachments[attachmentIndex], 0,
                    Utils::AsuraFBTextureFormatToGL(spec.TextureFormat), GL_INT, &value);
}
