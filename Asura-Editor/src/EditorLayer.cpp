#include "EditorLayer.h"
#include "glm/gtc/type_ptr.hpp"
#include "Renderer/RenderCommand.h"
#include "imgui/imgui.h"
#include "Core/Application.h"
#include "Core/Core.h"
#include "Utils/PlatformUtils.h"
#include "ImGuizmo.h"
#include "Math/Math.h"


Asura::EditorLayer::EditorLayer(): Asura::Layer("Editor Layer") {
    m_cameraController =
            std::make_shared<Asura::OrthographicCameraController>(1280.f/720.f, true);

    Asura::FrameBufferSpecification spec;
    spec.Width = 1280;
    spec.Height = 720;
    spec.Attachments = {Asura::FrameBufferTextureFormat::RGBA8,
                        Asura::FrameBufferTextureFormat::RED_INTEGER,
                        Asura::FrameBufferTextureFormat::Depth};
    m_frameBuffer = Asura::FrameBuffer::createFrameBuffer(spec);
}

Asura::EditorLayer::~EditorLayer() {

}

void Asura::EditorLayer::onAttach() {
    m_editorScene = createRef<Scene>();
    m_activeScene = m_editorScene;
    m_hierarchyPanel.setContext(m_editorScene);
    m_editorCamera = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);

    auto commandLineArgs = Asura::Application::getApplication()->getSpecification().CommandLineArgs;
    if (commandLineArgs.Count > 1) {
        auto sceneFilePath = commandLineArgs[1];
        SceneSerializer serializer(m_activeScene);
        serializer.deserialize(sceneFilePath);
    }

    m_playIcon = Texture2D::createTexture2D("../../Asura-Editor/resources/icons/PlayButton.png");
    m_stopIcon = Texture2D::createTexture2D("../../Asura-Editor/resources/icons/StopButton.png");
    m_simulateIcon = Texture2D::createTexture2D("../../Asura-Editor/resources/icons/SimulateButton.png");

    Asura::Renderer2D::setLineWidth(4.f);
}

void Asura::EditorLayer::onDetach() {

}

void Asura::EditorLayer::onUpdate(Asura::Timestep timestep) {
    using namespace Asura;

    // bind camera controller for every runtime camera
    m_activeScene->getRegistry().view<CameraComponent>().each([=](auto entityID, CameraComponent& cameraComponent) {
        Entity entity = {entityID, m_activeScene.get()};
        if(!entity.hasComponent<NativeScriptComponent>()) {
            entity.addComponent<NativeScriptComponent>().bind<CameraController>();
        }
    });

    if(FrameBufferSpecification spec = m_frameBuffer->getSpecification(); m_viewportSize.x > 0.0f &&
                                                                           m_viewportSize.y > 0.0f &&
                                                                           (spec.Width != m_viewportSize.x ||
                                                                            spec.Height != m_viewportSize.y)) {
        m_frameBuffer->resize((uint32_t) m_viewportSize.x, (uint32_t) m_viewportSize.y);
        m_cameraController->onResize(m_viewportSize.x, m_viewportSize.y);
        m_editorCamera.setViewportSize(m_viewportSize.x, m_viewportSize.y);
        m_activeScene->onViewportResize((uint32_t)m_viewportSize.x, (uint32_t)m_viewportSize.y);
    }

    Asura::Renderer2D::resetStats();

    m_frameBuffer->bind();
    RenderCommand::setClearColor({.1f, .1f, .1f, 1.f});
    RenderCommand::clear();

    m_frameBuffer->clearAttachment(1, -1);

    switch (m_sceneState) {
        case SceneState::Edit: {
            m_editorCamera.onUpdate(timestep);
            m_activeScene->onUpdateEditor(timestep, m_editorCamera);
            break;
        }
        case SceneState::Play: {
            m_activeScene->onUpdateRuntime(timestep);
            break;
        }
        case SceneState::Simulate: {
            m_editorCamera.onUpdate(timestep);
            m_activeScene->onUpdateSimulation(timestep, m_editorCamera);
            break;
        }
    }

    auto[mx, my] = ImGui::GetMousePos();
    mx -= m_viewportBounds[0].x;
    my -= m_viewportBounds[0].y;

    glm::vec2 viewportSize = m_viewportSize;
    my = viewportSize.y - my;
    int mouseX = (int)mx;
    int mouseY = (int)my;

    if(mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y) {
        int pixelData = m_frameBuffer->readPixel(1, mouseX, mouseY);
        m_hoveredEntity = pixelData == -1 ? Entity() :
                Entity((entt::entity)pixelData, m_activeScene.get());
    }

    onOverlayRender();

    m_frameBuffer->unbind();
}

void Asura::EditorLayer::onEvent(Asura::Event &event) {
    if(m_blockEvent) {
        ImGuiIO& io = ImGui::GetIO();
        event.setHandled(event.handled() | event.isInCategory(EventCategoryMouse) & io.WantCaptureMouse);
        event.setHandled(event.handled() | event.isInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard);
    }

    Asura::EventDispatcher dispatcher(event);
    dispatcher.dispatch<Asura::KeyPressedEvent>(BIND_EVENT_FUN(EditorLayer::onKeyPressed));
    dispatcher.dispatch<Asura::MouseButtonPressedEvent>(BIND_EVENT_FUN(EditorLayer::onMouseButtonPressed));

    if(m_sceneState == SceneState::Edit || m_sceneState == SceneState::Simulate) {
        m_editorCamera.onEvent(event);
    }
}

void Asura::EditorLayer::onImGuiRender() {
    ASURA_PROFILE_FUNCTION();

    static bool dockspaceOpen = true;
    static bool opt_fullscreen_persistant = true;
    bool opt_fullscreen = opt_fullscreen_persistant;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen) {
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }

    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
    ImGui::PopStyleVar();

    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    // DockSpace
    ImGuiIO& io = ImGui::GetIO();
    ImGuiStyle& style = ImGui::GetStyle();
    float minWinSizeX = style.WindowMinSize.x;
    style.WindowMinSize.x = 370.0f;

    if(io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }
    style.WindowMinSize.x = minWinSizeX;

    if(ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("New", "Ctrl+N"))
                newScene();

            if (ImGui::MenuItem("Open...", "Ctrl+O"))
                openScene();

            if (ImGui::MenuItem("Save", "Ctrl+S"))
                saveScene();

            if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
                saveSceneAs();

            if(ImGui::MenuItem("Exit")) {
                Asura::Application::getApplication()->close();
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    m_hierarchyPanel.onImGuiRender();
    m_contentBrowserPanel.onImGuiRender();

    {
        ImGui::Begin("Stats");

        std::string name = "None";
        if(m_hoveredEntity)
            name = m_hoveredEntity.hasComponent<TagComponent>() ?
                    m_hoveredEntity.getComponent<TagComponent>().Tag : "None";
        ImGui::Text("Hovered Entity: %s", name.c_str());

        auto stats = Asura::Renderer2D::getStats();
        ImGui::Text("Renderer2D Stats");
        ImGui::Text("Draw Calls: %d", stats.DrawCalls);
        ImGui::Text("Quads: %d", stats.QuadCount);
        ImGui::Text("Vertices: %d", stats.getTotalVertexCount());
        ImGui::Text("Indices: %d", stats.getTotalIndexCount());

        ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
        ImGui::End();
    }

    {
        ImGui::Begin("Settings");
        ImGui::Checkbox("Show physics colliders", &m_showPhysicsColliders);
        ImGui::End();
    }

    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
        ImGui::Begin("Viewport");

        auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
        auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
        auto viewportOffset = ImGui::GetWindowPos();
        m_viewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
        m_viewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

        m_viewportFocused = ImGui::IsWindowFocused();
        m_viewportHovered = ImGui::IsWindowHovered();
        m_blockEvent = !(m_viewportFocused & m_viewportHovered);

        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        m_viewportSize = { viewportPanelSize.x, viewportPanelSize.y };
        uint64_t textureID = m_frameBuffer->getColorAttachmentRendererID();
        ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ m_viewportSize.x, m_viewportSize.y },
                     ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

        if(ImGui::BeginDragDropTarget()) {
            if(const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {
                const wchar_t* path = (const wchar_t*)payload->Data;
                openScene(std::filesystem::path(Asura::getAssetPath()) / path);
            }
            ImGui::EndDragDropTarget();
        }

        // Gizmos
        Entity selectedEntity = m_hierarchyPanel.getSelectedEntity();
        if(selectedEntity && m_gizmoType != -1) {
            ImGuizmo::SetOrthographic(false);
            ImGuizmo::SetDrawlist();

            ImGuizmo::SetRect(m_viewportBounds[0].x, m_viewportBounds[0].y,
                              m_viewportBounds[1].x - m_viewportBounds[0].x,
                              m_viewportBounds[1].y - m_viewportBounds[0].y);

            const glm::mat4& cameraProjection = m_editorCamera.getProjection();
            glm::mat4 cameraView = m_editorCamera.getViewMatrix();

            // Entity transform
            auto& tc = selectedEntity.getComponent<TransformComponent>();
            glm::mat4 transform = tc.getTransform();

            // Snapping
            bool snap = Input::isKeyPressed(Key::LeftControl);
            float snapValue = 0.5f; // Snap to 0.5m for translation/scale
            // Snap to 45 degrees for rotation
            if(m_gizmoType == ImGuizmo::OPERATION::ROTATE)
                snapValue = 45.0f;

            float snapValues[3] = { snapValue, snapValue, snapValue };

            ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
                                 (ImGuizmo::OPERATION)m_gizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
                                 nullptr, snap ? snapValues : nullptr);

            if(ImGuizmo::IsUsing()) {
                glm::vec3 translation, rotation, scale;
                Asura::Math::decomposeTransform(transform, translation, rotation, scale);

                glm::vec3 deltaRotation = rotation - tc.Rotation;
                tc.Translation = translation;
                tc.Rotation += deltaRotation;
                tc.Scale = scale;
            }
        }

        ImGui::End();
        ImGui::PopStyleVar();
    }

    uiToolbar();

    ImGui::End();
}

bool Asura::EditorLayer::onKeyPressed(Asura::KeyPressedEvent &event) {
    if(event.isRepeat())
        return false;

    bool control = Input::isKeyPressed(Key::LeftControl) || Input::isKeyPressed(Key::RightControl);
    bool shift = Input::isKeyPressed(Key::LeftShift) || Input::isKeyPressed(Key::RightShift);

    switch (event.getKeyCode()) {
        case Key::N: {
            if (control)
                newScene();
            break;
        }
        case Key::O: {
            if(control)
                openScene();
            break;
        }
        case Key::S: {
            if(control) {
                if(shift)
                    saveSceneAs();
                else
                    saveScene();
            }
            break;
        }


        // Scene
        case Key::D: {
            if(control)
                onDuplicateEntity();
            break;
        }


        // Gizmos
        case Key::Q: {
            if(!ImGuizmo::IsUsing())
                m_gizmoType = -1;
            break;
        }

        case Key::W: {
            if(!ImGuizmo::IsUsing())
                m_gizmoType = ImGuizmo::OPERATION::TRANSLATE;
            break;
        }

        case Key::E: {
            if(!ImGuizmo::IsUsing())
                m_gizmoType = ImGuizmo::OPERATION::ROTATE;
            break;
        }
        case Key::R: {
            if (!ImGuizmo::IsUsing())
                m_gizmoType = ImGuizmo::OPERATION::SCALE;
            break;
        }
    }
    return false;
}

void Asura::EditorLayer::newScene() {
    m_editorScene = Asura::createRef<Asura::Scene>();
    m_editorScene->onViewportResize((unsigned)m_viewportSize.x, (unsigned)m_viewportSize.y);
    m_hierarchyPanel.setContext(m_editorScene);
    m_activeScene = m_editorScene;

    m_editorScenePath = std::filesystem::path();
}

void Asura::EditorLayer::openScene() {
    std::string filepath = FileDialogs::openFile();
    if (!filepath.empty())
        openScene(filepath);
}

void Asura::EditorLayer::openScene(const std::filesystem::path& path) {
    if(m_sceneState != SceneState::Edit)
        onSceneStop();

    if(path.extension().string() != ".asura") {
        ASURA_WARN("Could not load {0} - not a scene file", path.filename().string());
        return;
    }
    Ref<Scene> newScene = createRef<Scene>();
    SceneSerializer serializer(newScene);
    if(serializer.deserialize(path.string())) {
        m_editorScene = newScene;
        m_editorScene->onViewportResize((uint32_t)m_viewportSize.x, (uint32_t)m_viewportSize.y);
        m_hierarchyPanel.setContext(m_editorScene);

        m_activeScene = m_editorScene;
        m_editorScenePath = path;
    }
}

void Asura::EditorLayer::saveScene() {
    if(!m_editorScenePath.empty())
        serializeScene(m_activeScene, m_editorScenePath);
    else
        saveSceneAs();
}

void Asura::EditorLayer::saveSceneAs() {
    std::string filepath = FileDialogs::saveFile();
    if(!filepath.empty()) {
        serializeScene(m_activeScene, filepath);
        m_editorScenePath = filepath;
    }
}

bool Asura::EditorLayer::onMouseButtonPressed(Asura::MouseButtonPressedEvent &event) {
    if(event.getMouseButton() == Mouse::ButtonLeft) {
        if (m_viewportHovered && !ImGuizmo::IsOver() && !Input::isKeyPressed(Key::LeftAlt))
            m_hierarchyPanel.setSelectedEntity(m_hoveredEntity);
    }
    return false;
}

void Asura::EditorLayer::uiToolbar() {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
    auto& colors = ImGui::GetStyle().Colors;
    const auto& buttonHovered = colors[ImGuiCol_ButtonHovered];
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(buttonHovered.x, buttonHovered.y, buttonHovered.z, 0.5f));
    const auto& buttonActive = colors[ImGuiCol_ButtonActive];
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(buttonActive.x, buttonActive.y, buttonActive.z, 0.5f));

    ImGui::Begin("##toolbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

    float size = ImGui::GetWindowHeight() - 4.0f;
    ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));

    uint64_t playTextureID = m_playIcon->getRendererID();
    uint64_t stopTextureID = m_stopIcon->getRendererID();
    uint64_t simulateTextureID = m_simulateIcon->getRendererID();

    bool toolbarEnabled = (bool)m_activeScene;

    ImVec4 tintColor = ImVec4(1, 1, 1, 1);
    if (!toolbarEnabled)
        tintColor.w = 0.5f;

    {
        auto id = (m_sceneState == SceneState::Edit || m_sceneState == SceneState::Simulate) ? playTextureID : stopTextureID;
        ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));
        if (ImGui::ImageButton((ImTextureID)id, ImVec2(size, size), ImVec2(0, 0),
                               ImVec2(1, 1), 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f),
                               tintColor) && toolbarEnabled) {
            if(m_sceneState == SceneState::Edit || m_sceneState == SceneState::Simulate)
                onScenePlay();
            else if(m_sceneState == SceneState::Play) {
                onSceneStop();
            }
        }
    }

    ImGui::SameLine();
    {
        auto id = (m_sceneState == SceneState::Edit || m_sceneState == SceneState::Play) ? simulateTextureID : stopTextureID;
        if (ImGui::ImageButton((ImTextureID)id, ImVec2(size, size), ImVec2(0, 0),
                               ImVec2(1, 1), 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f),
                               tintColor) && toolbarEnabled) {
            if (m_sceneState == SceneState::Edit || m_sceneState == SceneState::Play)
                onSceneSimulate();
            else if(m_sceneState == SceneState::Simulate)
                onSceneStop();
        }
    }

    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor(3);
    ImGui::End();
}

void Asura::EditorLayer::onScenePlay() {
    if(m_sceneState == SceneState::Simulate) {
        onSceneStop();
        return;
    }

    m_sceneState = SceneState::Play;
    m_activeScene = Scene::copy(m_editorScene);
    m_activeScene->onRuntimeStart();

    m_hierarchyPanel.setContext(m_activeScene);
}

void Asura::EditorLayer::onSceneStop() {
    if(m_sceneState == SceneState::Play)
        m_activeScene->onRuntimeStop();
    else if(m_sceneState == SceneState::Simulate)
        m_activeScene->onSimulationStop();

    m_sceneState = SceneState::Edit;
    m_activeScene->onRuntimeStop();
    m_activeScene = m_editorScene;
    m_hierarchyPanel.setContext(m_activeScene);
}

void Asura::EditorLayer::serializeScene(Asura::Ref<Asura::Scene> scene, const std::filesystem::path &path) {
    SceneSerializer serializer(scene);
    serializer.serialize(path.string());
}

void Asura::EditorLayer::onDuplicateEntity() {
    if(m_sceneState != SceneState::Edit)
        return;

    Entity selectedEntity = m_hierarchyPanel.getSelectedEntity();
    if (selectedEntity)
        m_editorScene->duplicateEntity(selectedEntity);
}

void Asura::EditorLayer::onOverlayRender() {
    if(m_sceneState == SceneState::Play) {
        Entity camera = m_activeScene->getPrimaryCameraEntity();
        if(!camera)
            return;
        Renderer2D::beginScene(camera.getComponent<CameraComponent>().Camera,
                               camera.getComponent<TransformComponent>().getTransform());
    }
    else {
        Renderer2D::beginScene(m_editorCamera);
    }

    if (m_showPhysicsColliders) {
        {
            // Box Colliders
            auto view = m_activeScene->getAllEntitiesWith<TransformComponent, BoxCollider2DComponent>();
            for(auto entity : view) {
                auto [tc, bc2d] =
                        view.get<TransformComponent, BoxCollider2DComponent>(entity);

                glm::vec3 translation = tc.Translation + glm::vec3(bc2d.Offset, 0.001f);
                glm::vec3 scale = tc.Scale * glm::vec3(bc2d.Size * 2.0f, 1.0f);

                glm::mat4 transform = glm::translate(glm::mat4(1.0f), translation)
                                      * glm::rotate(glm::mat4(1.0f), tc.Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f))
                                      * glm::scale(glm::mat4(1.0f), scale);

                Renderer2D::drawRect(transform, glm::vec4(0, 1, 0, 1));
            }
        }

        // Circle Colliders
        {
            auto view = m_activeScene->getAllEntitiesWith<TransformComponent, CircleCollider2DComponent>();
            for (auto entity : view)
            {
                auto [tc, cc2d] = view.get<TransformComponent, CircleCollider2DComponent>(entity);

                glm::vec3 translation = tc.Translation + glm::vec3(cc2d.Offset, 0.001f);
                glm::vec3 scale = tc.Scale * glm::vec3(cc2d.Radius * 2.0f);

                glm::mat4 transform = glm::translate(glm::mat4(1.0f), translation)
                                      * glm::scale(glm::mat4(1.0f), scale);

                Renderer2D::drawCircle(transform, glm::vec4(0, 1, 0, 1), 0.1f);
            }
        }
    }

    if(Entity selectedEntity = m_hierarchyPanel.getSelectedEntity()) {
        const TransformComponent& transform = selectedEntity.getComponent<TransformComponent>();
        Renderer2D::drawRect(transform.getTransform(), glm::vec4(1.0f, 0.5f, 0.0f, 1.0f));
    }


    Renderer2D::endScene();
}

void Asura::EditorLayer::onSceneSimulate() {
    if(m_sceneState == SceneState::Play)
        onSceneStop();

    m_sceneState = SceneState::Simulate;
    m_activeScene = Scene::copy(m_editorScene);
    m_activeScene->onSimulationStart();
    m_hierarchyPanel.setContext(m_activeScene);
}
