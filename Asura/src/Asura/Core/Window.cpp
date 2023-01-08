#include "Core/Window.h"

#ifdef ASURA_PLATFORM_WINDOWS
    #include "Platform/Windows/WindowsWindow.h"
#endif


Asura::Scope<Asura::Window> Asura::Window::createWindow(const Asura::WindowProps &props) {
    #ifdef ASURA_PLATFORM_WINDOWS
            return createScope<WindowsWindow>(props);
    #else
            HZ_CORE_ASSERT(false, "Unknown platform!");
            return nullptr;
    #endif
}
