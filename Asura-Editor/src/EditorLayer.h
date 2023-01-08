// @author: lx
// @time: 2022/10/26
// @brief: 

#ifndef GAME_EDITORLAYER_H
#define GAME_EDITORLAYER_H

#include "Asura/Core/Core.h"
#include "Core/Timestep.h"
#include "Asura/Core/Layer.h"
#include "Asura/Renderer/OrthographicCameraController.h"
#include "Renderer/Renderer2D.h"
#include "Debug/Instrumentor.h"
#include "Platform/OpenGL/OpenGLFrameBuffer.h"
#include "Scene/Scene.h"
#include "Scene/Components.h"
#include "Scene/Entity.h"
#include "Scene/ScriptableEntity.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Scene/SceneSerializer.h"
#include "Events/KeyEvent.h"
#include "Renderer/EditorCamera.h"
#include "Panels/ContentBrowserPanel.h"


class CameraController : public Asura::ScriptableEntity {
public:
    void onCreate() {}
    void onDestroy() {}

    void onUpdate(Asura::Timestep timestep) {
        auto transform = getComponent<Asura::TransformComponent>();
    }
};


namespace Asura {
    extern const std::filesystem::path s_assetPath;

    class EditorLayer : public Layer {
    public:
        EditorLayer();
        ~EditorLayer() override;

        void onAttach() override;
        void onDetach() override;

        void onUpdate(Asura::Timestep timestep) override;
        void onImGuiRender() override;
        void onEvent(Asura::Event &event) override;
        void onOverlayRender();

    private:
        Ref<Asura::OrthographicCameraController> m_cameraController;

        Ref<FrameBuffer> m_frameBuffer;

        glm::vec2 m_viewportSize = { 0.0f, 0.0f };

        bool m_viewportFocused = false, m_viewportHovered = false;
        bool m_blockEvent = false;

        Ref<Scene> m_activeScene;
        Ref<Scene> m_editorScene;
        std::filesystem::path m_editorScenePath;

        class CameraController : public ScriptableEntity {
        public:
            void onCreate() override {
            }
            void onDestroy() override {}
            void onUpdate(Timestep timestep) override {
                auto& translation = getComponent<TransformComponent>().Translation;
                float speed = 5.f;
                if(Input::isKeyPressed(Key::W))
                    translation.y += timestep * speed;
                if(Input::isKeyPressed(Key::S))
                    translation.y -= timestep * speed;
                if(Input::isKeyPressed(Key::A))
                    translation.x -= timestep * speed;
                if(Input::isKeyPressed(Key::D))
                    translation.x += timestep * speed;

                if(Input::isKeyPressed(Key::Z)) {
                    translation.z -= timestep * speed;
                }
                if(Input::isKeyPressed(Key::C)) {
                    translation.z += timestep * speed;
                }
            }
        };

        SceneHierarchyPanel m_hierarchyPanel;
        ContentBrowserPanel m_contentBrowserPanel;

        int m_gizmoType = -1;
        EditorCamera m_editorCamera;

        glm::vec2 m_viewportBounds[2];

        Entity m_hoveredEntity;

        Ref<Texture2D> m_playIcon, m_stopIcon, m_simulateIcon;
        enum class SceneState {
            Edit = 0,
            Play = 1,
            Simulate = 2,
        };
        SceneState m_sceneState = SceneState::Edit;

        bool m_showPhysicsColliders = false;

    private:
        bool onKeyPressed(KeyPressedEvent& event);
        bool onMouseButtonPressed(MouseButtonPressedEvent& event);

        void newScene();
        void openScene();
        void openScene(const std::filesystem::path& path);
        void saveScene();
        void saveSceneAs();

        void onScenePlay();
        void onSceneStop();
        void onSceneSimulate();

        void uiToolbar();
        void onDuplicateEntity();

        void serializeScene(Ref<Scene> scene, const std::filesystem::path& path);
    };
}


#endif //GAME_EDITORLAYER_H
