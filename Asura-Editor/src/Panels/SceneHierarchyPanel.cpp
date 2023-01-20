#include "SceneHierarchyPanel.h"
#include "imgui/imgui.h"
#include "Scene/Components.h"
#include "imgui/imgui_internal.h"
#include "ContentBrowserPanel.h"


Asura::SceneHierarchyPanel::SceneHierarchyPanel(const Asura::Ref<Asura::Scene> &context) {
    setContext(context);
}

void Asura::SceneHierarchyPanel::setContext(const Asura::Ref<Asura::Scene> &context) {
    m_context = context;
    m_selectionContext = {};
}

void Asura::SceneHierarchyPanel::onImGuiRender() {
    ImGui::Begin("Scene Hierarchy");

    if(m_context) {
        m_context->m_registry.each([&](auto entityID) {
            Entity entity{entityID , m_context.get()};
            drawEntityNode(entity);
        });

        if(ImGui::IsMouseDown(0) && ImGui::IsWindowHovered()) {
            m_selectionContext = {};
        }

        if(ImGui::BeginPopupContextWindow(nullptr, 1)) {
            if(ImGui::MenuItem("Create empty entity")) {
                m_selectionContext = m_context->createEntity("NewEntity");
            }
            ImGui::EndPopup();
        }
        ImGui::End();
    }


    ImGui::Begin("Properties");
    if(m_selectionContext) {
        ImGui::Text("GUID: %u", (uint64_t)m_selectionContext.getGUID());
        drawComponents(m_selectionContext);
    }
    ImGui::End();
}

void Asura::SceneHierarchyPanel::drawEntityNode(Asura::Entity entity) {
    auto& tag = entity.getComponent<Asura::TagComponent>().Tag;

    ImGuiTreeNodeFlags flags = ((m_selectionContext == entity) ?
            ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
    flags |= ImGuiTreeNodeFlags_SpanAvailWidth;

    bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());

    if(ImGui::IsItemClicked()) {
        m_selectionContext = entity;
    }

    bool entityDeleted = false;
    if(ImGui::BeginPopupContextItem()) {
        if(ImGui::MenuItem("Delete Entity"))
            entityDeleted = true;
        ImGui::EndPopup();
    }

    if(opened) {
        flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
        opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
        if(opened) {
            ImGui::TreePop();
        }
        ImGui::TreePop();
    }

    if(entityDeleted) {
        m_context->destroyEntity(entity);
        if(m_selectionContext == entity)
            m_selectionContext = {};
    }
}

void Asura::SceneHierarchyPanel::drawComponents(Asura::Entity entity) {
    if(entity.hasComponent<Asura::TagComponent>()) {
        auto& tag = entity.getComponent<TagComponent>().Tag;
        char buffer[256];
        memset(buffer, 0, sizeof(buffer));
        strcpy_s(buffer, sizeof(buffer), tag.c_str());
        if(ImGui::InputText("##Tag", buffer, sizeof(buffer)))
            tag = std::string(buffer);
    }

    ImGui::SameLine();
    ImGui::PushItemWidth(-1);

    if(ImGui::Button("Add Component"))
        ImGui::OpenPopup("AddComponent");

    if(ImGui::BeginPopup("AddComponent")) {
        if(!m_selectionContext.hasComponent<Asura::SpriteRendererComponent>()) {
            if(ImGui::MenuItem("SpriteRendererComponent")) {
                m_selectionContext.addComponent<Asura::SpriteRendererComponent>();
                ImGui::CloseCurrentPopup();
            }
        }

        if(!m_selectionContext.hasComponent<Asura::CircleRendererComponent>()) {
            if(ImGui::MenuItem("CircleRendererComponent")) {
                m_selectionContext.addComponent<Asura::CircleRendererComponent>();
                ImGui::CloseCurrentPopup();
            }
        }

        if(!m_selectionContext.hasComponent<Asura::CameraComponent>()) {
            if(ImGui::MenuItem("Camera")) {
                m_selectionContext.addComponent<Asura::CameraComponent>();
                ImGui::CloseCurrentPopup();
            }
        }

        if(!m_selectionContext.hasComponent<Asura::Rigidbody2DComponent>()) {
            if(ImGui::MenuItem("Rigidbody 2D")) {
                m_selectionContext.addComponent<Rigidbody2DComponent>();
                ImGui::CloseCurrentPopup();
            }
        }

        if(!m_selectionContext.hasComponent<BoxCollider2DComponent>()) {
            if (ImGui::MenuItem("Box Collider 2D"))
            {
                m_selectionContext.addComponent<BoxCollider2DComponent>();
                ImGui::CloseCurrentPopup();
            }
        }

        if(!m_selectionContext.hasComponent<CircleCollider2DComponent>()) {
            if (ImGui::MenuItem("Circle Collider 2D")) {
                m_selectionContext.addComponent<CircleCollider2DComponent>();
                ImGui::CloseCurrentPopup();
            }
        }

        ImGui::EndPopup();
    }
    ImGui::PopItemWidth();

    drawComponent<Asura::TransformComponent>("Transform", entity, [](auto& component) {
        drawVec3Control("Translation", component.Translation);

        glm::vec3 rotation = glm::degrees(component.Rotation);
        drawVec3Control("Rotation", rotation);
        component.Rotation = glm::radians(rotation);

        drawVec3Control("Scale", component.Scale, 1.0f);
    });

    drawComponent<Asura::SpriteRendererComponent>("Sprite Renderer", entity, [](auto& component){
        ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));

        ImGui::Button("Texture", ImVec2(100.0f, 0.0f));
        if(ImGui::BeginDragDropTarget()) {
            if(const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {
                const auto* path = (const wchar_t*)payload->Data;
                std::filesystem::path texturePath = std::filesystem::path(Asura::getAssetPath()) / path;

                Ref<Texture2D> texture = Texture2D::createTexture2D(texturePath.string());
                if(texture->isLoaded())
                    component.Texture = texture;
                else
                    ASURA_WARN("Could not load texture {0}", texturePath.filename().string());
            }
            ImGui::EndDragDropTarget();
        }
        ImGui::DragFloat("Tiling Factor", &component.TilingFactor, 0.1f, 0.0f, 100.0f);
    });

    drawComponent<Asura::CircleRendererComponent>("Circle Renderer", entity, [](auto& component) {
        ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));

        ImGui::DragFloat("Thickness", &component.Thickness, 0.05f, 0.0f, 1.0f);
        ImGui::DragFloat("Fade", &component.Fade, 0.1f, 0.0f, 10.0f);
    });

    drawComponent<Asura::CameraComponent>("Camera", entity, [](auto& component) {
        auto& camera = component.Camera;

        ImGui::Text("Aspect: Ratio: %.3f", camera.getAspectRatio());
        ImGui::Checkbox("Primary", &component.primary);
        const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
        const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.getProjectionType()];
        if(ImGui::BeginCombo("Projection", currentProjectionTypeString)) {
            for (int i = 0; i < 2; i++) {
                bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
                if(ImGui::Selectable(projectionTypeStrings[i], isSelected)) {
                    currentProjectionTypeString = projectionTypeStrings[i];
                    camera.setProjectionType((SceneCamera::ProjectionType)i);
                }
                if(isSelected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }

        if(camera.getProjectionType() == SceneCamera::ProjectionType::Perspective) {
            float perspectiveVerticalFov = camera.getPerspectiveVerticalFOV();
            if(ImGui::DragFloat("Vertical FOV", &perspectiveVerticalFov))
                camera.setPerspectiveVerticalFOV(perspectiveVerticalFov);

            float perspectiveNear = camera.getPerspectiveNearClip();
            if(ImGui::DragFloat("Near", &perspectiveNear))
                camera.setPerspectiveNearClip(perspectiveNear);

            float perspectiveFar = camera.getPerspectiveFarClip();
            if(ImGui::DragFloat("Far", &perspectiveFar))
                camera.setPerspectiveFarClip(perspectiveFar);
        }

        if(camera.getProjectionType() == SceneCamera::ProjectionType::Orthographic) {
            float orthoSize = camera.getOrthographicSize();
            if(ImGui::DragFloat("Size", &orthoSize))
                camera.setOrthographicSize(orthoSize);
            float orthoNear = camera.getOrthographicNearClip();
            if(ImGui::DragFloat("Near", &orthoNear))
                camera.setOrthographicNearClip(orthoNear);
            float orthoFar = camera.getOrthographicFarClip();
            if(ImGui::DragFloat("Far", &orthoFar))
                camera.setOrthographicFarClip(orthoFar);
            ImGui::Checkbox("Fixed Aspect Ratio", &component.fixedAspectRatio);
        }
    });

    drawComponent<Rigidbody2DComponent>("Rigidbody 2D", entity, [](auto& component) {
        const char* bodyTypeStrings[] = { "Static", "Dynamic", "Kinematic"};
        const char* currentBodyTypeString = bodyTypeStrings[(int)component.Type];
        if(ImGui::BeginCombo("Body Type", currentBodyTypeString)) {
            for (int i = 0; i < 2; i++) {
                bool isSelected = currentBodyTypeString == bodyTypeStrings[i];
                if(ImGui::Selectable(bodyTypeStrings[i], isSelected)) {
                    currentBodyTypeString = bodyTypeStrings[i];
                    component.Type = (Rigidbody2DComponent::BodyType)i;
                }
                if(isSelected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }

        ImGui::Checkbox("Fixed Rotation", &component.FixedRotation);
    });


    drawComponent<BoxCollider2DComponent>("Box Collider 2D", entity, [](auto& component) {
        ImGui::DragFloat2("Offset", glm::value_ptr(component.Offset));
        ImGui::DragFloat2("Size", glm::value_ptr(component.Size));
        ImGui::DragFloat("Density", &component.Density, 0.01f, 0.0f, 1.0f);
        ImGui::DragFloat("Friction", &component.Friction, 0.01f, 0.0f, 1.0f);
        ImGui::DragFloat("Restitution", &component.Restitution, 0.01f, 0.0f, 1.0f);
        ImGui::DragFloat("Restitution Threshold", &component.RestitutionThreshold, 0.01f, 0.0f);
    });

    drawComponent<CircleCollider2DComponent>("Circle Collider 2D", entity, [](auto& component) {
        ImGui::DragFloat2("Offset", glm::value_ptr(component.Offset));
        ImGui::DragFloat("Radius", &component.Radius);
        ImGui::DragFloat("Density", &component.Density, 0.01f, 0.0f, 1.0f);
        ImGui::DragFloat("Friction", &component.Friction, 0.01f, 0.0f, 1.0f);
        ImGui::DragFloat("Restitution", &component.Restitution, 0.01f, 0.0f, 1.0f);
        ImGui::DragFloat("Restitution Threshold", &component.RestitutionThreshold, 0.01f, 0.0f);
    });
}

void Asura::SceneHierarchyPanel::drawVec3Control(const std::string &label, glm::vec3 &values, float resetValue,
                                                 float columnWidth) {
    ImGuiIO& io = ImGui::GetIO();
    auto boldFont = io.Fonts->Fonts[0];

    ImGui::PushID(label.c_str());

    ImGui::Columns(2);
    ImGui::SetColumnWidth(0, columnWidth);
    ImGui::Text(label.c_str());
    ImGui::NextColumn();

    ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

    float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
    ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });

    ImGui::PushFont(boldFont);
    if(ImGui::Button("X", buttonSize))
        values.x = resetValue;
    ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
    ImGui::PopItemWidth();
    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
    ImGui::PushFont(boldFont);
    if(ImGui::Button("Y", buttonSize))
        values.y = resetValue;
    ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
    ImGui::PopItemWidth();
    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
    ImGui::PushFont(boldFont);
    if(ImGui::Button("Z", buttonSize))
        values.z = resetValue;
    ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
    ImGui::PopItemWidth();

    ImGui::PopStyleVar();

    ImGui::Columns(1);

    ImGui::PopID();
}

template<typename T, typename UIFunction>
void Asura::SceneHierarchyPanel::drawComponent(const std::string &name, Asura::Entity entity, UIFunction uiFunction) {
    const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed |
            ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiButtonFlags_AllowItemOverlap |
            ImGuiStyleVar_FramePadding;
    if(entity.hasComponent<T>()) {
        auto& component = entity.getComponent<T>();
        ImVec2 contentRegionAvail = ImGui::GetContentRegionAvail();

        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{4, 4});
        float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.f;

        ImGui::Separator();
        bool open = ImGui::TreeNodeEx((void*) typeid(T).hash_code(), treeNodeFlags, name.c_str());
        ImGui::PopStyleVar();

        ImGui::SameLine(contentRegionAvail.x - lineHeight * .5f);
        if(ImGui::Button("+", ImVec2{lineHeight, lineHeight})) {
            ImGui::OpenPopup("ComponentSettings");
        }

        bool removeComponent = false;
        if(ImGui::BeginPopup("ComponentSettings")) {
            if(ImGui::MenuItem("Remove component"))
                removeComponent = true;
            ImGui::EndPopup();
        }

        if(open) {
            uiFunction(component);
        }

        if(removeComponent)
            entity.removeComponent<T>();
    }
}



