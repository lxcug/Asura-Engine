// @author: lx
// @time: 2022/10/26
// @brief: 

#pragma once
#ifndef GAME_ASURAEDITORAPP_H
#define GAME_ASURAEDITORAPP_H

#include "Asura.h"
#include "EditorLayer.h"


class AsuraEditor : public Asura::Application {
public:
    AsuraEditor(Asura::ApplicationSpecification& spec): Asura::Application(spec) {
        createImGuiContext();
        pushLayer(new Asura::EditorLayer);
    }

    ~AsuraEditor() override {
        destroyImGuiContext();
    }

    void createImGuiContext() override;
    void destroyImGuiContext() override;

    void beginImGui() const override;
    void endImGui() const override;

    void run() override;
    void close() override {
        m_running = false;
    }
    void onEvent(Asura::Event &event) override;

    bool onWindowClose(Asura::WindowCloseEvent &event);
    bool onWindowResize(Asura::WindowResizeEvent &event);

private:
    bool m_running = true;
    float m_lastFrameTime = 0.f;
    bool m_minimized = false;

    void setDarkThemeColors();
};

Asura::Application* Asura::createApplication(Asura::ApplicationCommandLineArgs args) {
    ApplicationSpecification spec;
    spec.Name = "Asura-Editor";
    spec.WorkingDirectory = "../../Asura-Editor";
    spec.CommandLineArgs = args;

    return new AsuraEditor(spec);
}


#endif //GAME_ASURAEDITORAPP_H
