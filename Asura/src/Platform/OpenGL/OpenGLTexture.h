// @author: lx
// @time: 2022/10/17
// @brief: 

#pragma once
#ifndef GAME_OPENGLTEXTURE_H
#define GAME_OPENGLTEXTURE_H

#include "Asura/Core/Core.h"
#include "Renderer/Texture.h"
#include "stb_image/stb_image.h"


namespace Asura {
    class ASURA_API OpenGLTexture2D : public Texture2D {
    public:
        OpenGLTexture2D(std::string path);
        OpenGLTexture2D(unsigned width, unsigned height);
        ~OpenGLTexture2D() override;

        unsigned getWidth() const override {
            return m_width;
        }
        unsigned getHeight() const override {
            return m_height;
        }
        unsigned getRendererID() const override {
            return m_rendererID;
        }

        void bind(unsigned slot=0) override;
        void unbind() override;

        const std::string& getPath() const override {
            return m_path;
        }

        bool isLoaded() const override {
            return m_isLoaded;
        }

        void SetData(void* data, uint32_t size) override;

    private:
        std::string m_path;
        unsigned m_width, m_height;
        unsigned m_rendererID;
        unsigned m_internalFormat, m_dataFormat;

        bool m_isLoaded = false;
    };
}


#endif //GAME_OPENGLTEXTURE_H
