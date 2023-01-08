// @author: lx
// @time: 2022/10/11
// @brief: 

#ifndef GAME_APPLICATION_H
#define GAME_APPLICATION_H

#include "Core.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"
#include "Events/ApplicationEvent.h"
#include "Platform/Windows/WindowsWindow.h"
#include "imgui/imgui.h"
#include "Renderer/OrthographicCamera.h"
#include "ImGui/backends/imgui_impl_glfw.h"
#include "ImGui/backends/imgui_impl_opengl3.h"
#include "LayerStack.h"
#include "Input.h"
#include "Platform/OpenGL/OpenGLBuffer.h"
#include "Renderer/Shader.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"
#include "Renderer/Renderer.h"


int main(int argc, char** argv);

namespace Asura {
    struct ApplicationCommandLineArgs {
        int Count = 0;
        char** Args = nullptr;

        const char* operator[](int index) const {
            ASURA_CORE_ASSERT(index < Count, "");
            return Args[index];
        }
    };

    struct ApplicationSpecification
    {
        std::string Name = "Asura Application";
        std::string WorkingDirectory;
        ApplicationCommandLineArgs CommandLineArgs;
    };


    class ASURA_API Application {
    public:
        /**
         * in constructor of Application, we initialize s_instance(singleton instance of Application) and create platform
         * specific window.
         */
        Application(const ApplicationSpecification& specification);
        virtual ~Application();

        /**
         * @brief a loop function, in this function, everything get updated and rendered.
         */
        virtual void run() = 0;

        /*!
         * @brief in constructor of Application, this function is bind to callback function of class Window, and when event
         * propagates from platform specific window(for example glfw), glfwSetEventCallback and call onEvent(this function),
         * Application receives the event and dispatches the event to layers in layerStack.
         * @param event
         */
        virtual void onEvent(Event& event) = 0;

        /**
         * @brief push a layer to layerStack, the memory of layer is managed by layerStack, so don't delete layer pointer
         * only if the layer is popped from layerStack.
         * @param layer
         */
        void pushLayer(Layer* layer);
        /**
         * @brief push a overlay to layerStack
         * @param overlay
         */
        void pushOverlay(Layer* overlay);

        /**
         * @brief get window (base class defined in Window.h)
         * @return Window*
         */
        inline Window* getWindow() {
            return m_window.get();
        }

        /**
         * @brief return singleton instance of Application
         * @return the pointer of singleton instance of Application
         */
        static Application* getApplication() {
            return s_instance;
        }

        virtual void close() = 0;

        virtual void beginImGui() const = 0;
        virtual void endImGui() const = 0;
        virtual void createImGuiContext() = 0;
        virtual void destroyImGuiContext() = 0;

        const ApplicationSpecification& getSpecification() const {
            return m_specification;
        }


    protected:
        static Application* s_instance;
        Scope<Window> m_window;
        LayerStack m_layerStack;

        ApplicationSpecification m_specification;
    };

    // implements by client to create user-define window
    Application* createApplication(ApplicationCommandLineArgs args);
}


#endif //GAME_APPLICATION_H
