// @author: lx
// @time: 2022/10/20
// @brief: 

#pragma once
#ifndef GAME_SANDBOXAPP_H
#define GAME_SANDBOXAPP_H

#include "Asura.h"
#include "Sandbox2D.h"


class Sandbox : public Asura::Application {
public:
    Sandbox(const Asura::ApplicationSpecification& specification): Asura::Application(specification) {
        createImGuiContext();
        pushLayer(new Sandbox2D);
    }

    ~Sandbox() override {
        destroyImGuiContext();
    }

    void createImGuiContext() override;
    void destroyImGuiContext() override;

    void beginImGui() const override;
    void endImGui() const override;

    void onEvent(Asura::Event &event) override;
    void run() override;
    void close() override {
        m_running = false;
    }

    bool onWindowClose(Asura::WindowCloseEvent &event);
    bool onWindowResize(Asura::WindowResizeEvent &event);

private:
    bool m_running = true;
    float m_lastFrameTime = 0.f;
    bool m_minimized = false;
};


Asura::Application* Asura::createApplication(Asura::ApplicationCommandLineArgs args) {
    ApplicationSpecification spec;
    spec.Name = "Sandbox";
    spec.WorkingDirectory = "../../Sandbox";
    spec.CommandLineArgs = args;

    return new Sandbox(spec);
}


#endif //GAME_SANDBOXAPP_H
