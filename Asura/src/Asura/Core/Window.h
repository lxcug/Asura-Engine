// @author: lx
// @time: 2022/10/12
// @brief: 

#ifndef GAME_WINDOW_H
#define GAME_WINDOW_H

#include "Asura/Core/Core.h"
#include "Events/Event.h"


namespace Asura {
    // struct of Window Properties
    struct WindowProps {
        explicit WindowProps(std::string title="Asura Engine", unsigned width=1600, unsigned height=900):
        Title(std::move(title)), Width(width), Height(height) {}

        std::string Title;
        unsigned Width;
        unsigned Height;
    };

    // Window Interface
    class ASURA_API Window {
    public:
        using EventCallbackFun = std::function<void(Event&)>;

        virtual ~Window() = default;  // 析构函数不能被声明为纯虚函数, 因为会被派生类调用

        virtual void onUpdate() = 0;

        virtual unsigned getWidth() const = 0;
        virtual unsigned getHeight() const = 0;
        virtual const std::string& getWindowName() const = 0;

        virtual void setEventCallback(const EventCallbackFun& callback) = 0;

        // 垂直同步 Vertical Synchronization
        virtual void setVSync(bool enable) = 0;
        [[nodiscard]] virtual bool isVSync() const = 0;

        [[nodiscard]] virtual void* getNativeWindow() const = 0;

        static Scope<Window> createWindow(const WindowProps& props = WindowProps());
    };
}


#endif //GAME_WINDOW_H
