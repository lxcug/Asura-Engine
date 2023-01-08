// @author: lx
// @time: 2022/10/26
// @brief: 

#pragma once
#ifndef GAME_FRAMEBUFFER_H
#define GAME_FRAMEBUFFER_H

#include "Core/Core.h"

#include <vector>


namespace Asura {
    enum class FrameBufferTextureFormat {
        None = 0x0,
        RGBA8,
        RED_INTEGER,
        DEPTH24STENCIL8,

        Depth = DEPTH24STENCIL8
    };

    struct FrameBufferTextureSpecification {
        FrameBufferTextureSpecification() = default;
        FrameBufferTextureSpecification(FrameBufferTextureFormat format): TextureFormat(format) {}

        FrameBufferTextureFormat TextureFormat;
    };

    struct FrameBufferAttachmentSpecification {
        FrameBufferAttachmentSpecification() = default;
        FrameBufferAttachmentSpecification(std::initializer_list<FrameBufferTextureSpecification> attachments):
        Attachments(attachments) {}

        std::vector<FrameBufferTextureSpecification> Attachments;
    };


    struct FrameBufferSpecification {
        unsigned Width = 0, Height = 0;
        unsigned Samples = 1;
        FrameBufferAttachmentSpecification Attachments;

        bool SwapChainTarget = false;
    };

    class ASURA_API FrameBuffer {
    public:
        virtual void bind() = 0;
        virtual void unbind() = 0;

        virtual void resize(unsigned width, unsigned height) = 0;
        virtual int readPixel(uint32_t attachmentIndex, int x, int y) = 0;

        virtual void clearAttachment(uint32_t attachmentIndex, int value) = 0;

        [[nodiscard]] virtual unsigned getColorAttachmentRendererID(unsigned index = 0) const = 0;
        [[nodiscard]] virtual const FrameBufferSpecification& getSpecification() const = 0;

        static Ref<FrameBuffer> createFrameBuffer(const FrameBufferSpecification& spec);
    };
}


#endif //GAME_FRAMEBUFFER_H
