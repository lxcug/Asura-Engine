// @author: lx
// @time: 2022/10/17
// @brief: 

#pragma once
#ifndef GAME_TEXTURE_H
#define GAME_TEXTURE_H

#include "Asura/Core/Core.h"


namespace Asura {
    class ASURA_API Texture {
    public:
        virtual ~Texture() = default;

        virtual unsigned getWidth() const = 0;
        virtual unsigned getHeight() const = 0;

        virtual void bind(unsigned slot) = 0;
        virtual void unbind() = 0;

        virtual const std::string& getPath() const = 0;

        virtual bool isLoaded() const = 0;

        virtual unsigned getRendererID() const = 0;

        virtual void SetData(void* data, uint32_t size) = 0;
    };

    class ASURA_API Texture2D : public Texture {
    public:
        static Ref<Texture2D> createTexture2D(const std::string& path);
        static Ref<Texture2D> createTexture2D(uint32_t width, uint32_t height);
    };
}


#endif //GAME_TEXTURE_H
