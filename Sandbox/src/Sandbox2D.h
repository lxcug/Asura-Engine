// @author: lx
// @time: 2022/10/19
// @brief: 

#pragma once
#ifndef GAME_SANDBOX2D_H
#define GAME_SANDBOX2D_H

#include "Asura/Core/Core.h"
#include "Core/Timestep.h"
#include "Asura/Core/Layer.h"
#include "Asura/Renderer/OrthographicCameraController.h"
#include "Renderer/Renderer2D.h"
#include "Debug/Instrumentor.h"


class Sandbox2D : public Asura::Layer {
public:
    Sandbox2D();
    ~Sandbox2D() override = default;

    void onAttach() override;
    void onDetach() override;

    void onUpdate(Asura::Timestep timestep) override;
    void onEvent(Asura::Event &event) override;

    void onImGuiRender() override;

private:
    Asura::Ref<Asura::OrthographicCameraController> m_cameraController;
    glm::vec4 m_color = {.25f, .9f, .1f, .5f};
    Asura::Ref<Asura::Texture2D> m_texture;
};


#endif //GAME_SANDBOX2D_H
