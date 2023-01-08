// @author: lx
// @time: 2022/10/12
// @brief: 

#ifndef GAME_WINDOWSWINDOW_H
#define GAME_WINDOWSWINDOW_H

#include "Asura/Core/Core.h"
#include "Asura/Core/Log.h"
#include "Asura/Core/Window.h"
#include "Platform/OpenGL/OpenGLContext.h"


namespace Asura {
    // Windows window
    class ASURA_API WindowsWindow : public Window {
    public:
        explicit WindowsWindow(const WindowProps& windowProps);
        ~WindowsWindow() override;

        /**
         * @brief called every frame
         */
        void onUpdate() override;

        [[nodiscard]] unsigned int getWidth() const override {
            return m_data.Width;
        }
        [[nodiscard]] unsigned int getHeight() const override {
            return m_data.Height;
        }
        const std::string & getWindowName() const override {
            return m_data.Title;
        }

        /**
         * @brief set vertical synchronization(垂直同步)
         * @param enable true to enable VSync and asle set m_data.bVSync = enable
         */
        void setVSync(bool enable) override;

        /**
         *
         * @return whether m_window sets Vertical Sync
         */
        [[nodiscard]] bool isVSync() const override {
            return m_data.bVSync;
        }

        /*!
         * @brief called by Application
         * Application调用setEventCallback来绑定回调函数
         * WindowsWindow通过m_data.eventCallback()来调用绑定的回调函数
         * 因此Application对WindowsWindow是透明的
         * @param callback 回调函数, 用于处理事件
         */
        void setEventCallback(const Asura::Window::EventCallbackFun &callback) override;

        /**
         * @brief destroy glfw window
         */
        void shutDown();

        /**
         * @brief get glfw window
         * @return
         */
        [[nodiscard]] void* getNativeWindow() const override {
            return m_window;
        }


    private:
        GLFWwindow* m_window;  // glfw window
        /**
         * @brief doesn't need to delete m_context, cause its just hold a pointer to GLFWwindow
         */
        Scope<GraphicsContext> m_context;

        struct WindowData {
            std::string Title;
            unsigned Width, Height;
            bool bVSync;

            EventCallbackFun eventCallback;  // bind to Application::onEvent
        };

        WindowData m_data;

        static bool s_glfwInit;

    private:
        /*!
         * @brief initialize glfw and set glfw window
         * @param props windows properties
         */
        void init(const WindowProps& props = *(new WindowProps));

        void setGLFWWindowEventCallback();

        void setGLFWMouseEventCallback();

        void setGLFWKeyEventCallback();

        void setGLFWApplicationEventCallback();

        void setGLFWCharEventCallback();
    };
}


#endif //GAME_WINDOWSWINDOW_H
