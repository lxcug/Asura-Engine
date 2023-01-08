#include "SandboxApp.h"


void Sandbox::createImGuiContext() {
    ASURA_PROFILE_FUNCTION();
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

    // font path to be modified
    io.Fonts->AddFontFromFileTTF("E:/code/opengl/resources/fonts/consolas.ttf", 17.f, nullptr);

    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)m_window->getNativeWindow(), true);
    const char* glsl_version = "#version 450";
    ImGui_ImplOpenGL3_Init(glsl_version);
}

void Sandbox::destroyImGuiContext() {
    ASURA_PROFILE_FUNCTION();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void Sandbox::beginImGui() const {
    ASURA_PROFILE_FUNCTION();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void Sandbox::endImGui() const {
    ASURA_PROFILE_FUNCTION();
    ImGuiIO& io = ImGui::GetIO();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}

void Sandbox::run() {
    while(m_running) {
        auto time = (float)glfwGetTime();
        Asura::Timestep timestep = time - m_lastFrameTime;
        m_lastFrameTime = time;

        if(!m_minimized) {
            for(auto* layer : m_layerStack) {
                layer->onUpdate(timestep);
            }
        }

        beginImGui();
        for(auto* layer : m_layerStack) {
            layer->onImGuiRender();
        }
        endImGui();

        m_window->onUpdate();
    }
}

void Sandbox::onEvent(Asura::Event &event) {
    using namespace Asura;

    EventDispatcher eventDispatcher(event);
    // cause dispatch needs a functional object rather than a pure function pointer, we need to
    // use std::bind to convert a function pointer to a functional object
    eventDispatcher.dispatch<WindowCloseEvent>(BIND_EVENT_FUN(Sandbox::onWindowClose));
    eventDispatcher.dispatch<WindowResizeEvent>(BIND_EVENT_FUN(Sandbox::onWindowResize));

    // handle from the overlay
    for(auto it = m_layerStack.rbegin(); it != m_layerStack.rend(); it++) {
        (*it)->onEvent(event);
        if(event.handled()) {
            break;
        }
    }
}

bool Sandbox::onWindowClose(Asura::WindowCloseEvent &event) {
    m_running = false;
    return true;
}

bool Sandbox::onWindowResize(Asura::WindowResizeEvent &event) {
    if(event.getWidth() == 0 || event.getHeight() == 0) {
        m_minimized = true;
        return false;
    }
    m_minimized = false;
    Asura::Renderer::onWindowResize(event.getWidth(), event.getHeight());
    return false;
}
