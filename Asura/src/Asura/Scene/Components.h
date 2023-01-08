// @author: lx
// @time: 2022/10/26
// @brief: 

#ifndef GAME_COMPONENTS_H
#define GAME_COMPONENTS_H

#include "Core/Core.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Scene/SceneCamera.h"
#include "Core/Timestep.h"
#include "Renderer/Texture.h"
#include "Core/GUID.h"

#include <string>
#include <utility>
#include <functional>

#define GLM_ENABLE_EXPERIMENTAL
#include"glm/gtx/quaternion.hpp"


namespace Asura {
    struct TransformComponent {
        glm::vec3 Translation = {0.f, 0.f, 0.f};
        glm::vec3 Rotation = {0.f, 0.f, 0.f};
        glm::vec3 Scale = {1.f, 1.f, 1.f};

        TransformComponent() = default;
        TransformComponent(const TransformComponent&) = default;
        TransformComponent(const glm::vec3& translation): Translation(translation) {}

        glm::mat4 getTransform() const {
            glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));
            return glm::translate(glm::mat4(1.f), Translation) * rotation * glm::scale(glm::mat4(1.f), Scale);
        }
    };

    struct SpriteRendererComponent {
        glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };
        float TilingFactor = 1.f;
        Ref<Texture2D> Texture = nullptr;

        SpriteRendererComponent() = default;
        SpriteRendererComponent(const SpriteRendererComponent&) = default;
        SpriteRendererComponent(const glm::vec4& color): Color(color) {}
        SpriteRendererComponent(const glm::vec4& color, Ref<Texture2D>& texture): Color(color), Texture(texture) {}
    };

    struct CircleRendererComponent {
        glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };
        float Thickness = 1.0f;
        float Fade = 0.005f;

        CircleRendererComponent() = default;
        CircleRendererComponent(const CircleRendererComponent&) = default;
    };

    struct TagComponent {
        std::string Tag;

        TagComponent() = default;
        TagComponent(const TagComponent& tagComponent): Tag(tagComponent.Tag) {}
        TagComponent(std::string tag): Tag(std::move(tag)) {}
    };

    struct GUIDComponent {
        GUID ID;

        GUIDComponent() = default;
        GUIDComponent(GUID guid): ID(guid) {}
        GUIDComponent(const GUIDComponent&) = default;
    };

    struct CameraComponent {
        SceneCamera Camera;
        bool primary = true;
        bool fixedAspectRatio = false;

        CameraComponent() = default;
        CameraComponent(SceneCamera& camera): Camera(camera) {}
        CameraComponent(const CameraComponent&) = default;
    };

    class ScriptableEntity;

    struct NativeScriptComponent {
        ScriptableEntity* instance = nullptr;

        std::function<ScriptableEntity*()> instantiateScript;
        std::function<void(NativeScriptComponent*)> destroyScript;

        template<typename T>
        void bind() {
            instantiateScript = []() {
                return static_cast<ScriptableEntity*>(new T());
            };
            destroyScript = [](NativeScriptComponent* nsc) {
                delete nsc->instance;
                nsc->instance = nullptr;
            };
        }
    };


    // Physics
    struct Rigidbody2DComponent {
        enum class BodyType { Static = 0, Dynamic, Kinematic };
        BodyType Type = BodyType::Static;
        bool FixedRotation = false;

        // Storage for runtime
        void* RuntimeBody = nullptr;

        Rigidbody2DComponent() = default;
        Rigidbody2DComponent(const Rigidbody2DComponent&) = default;
    };

    struct BoxCollider2DComponent {
        glm::vec2 Offset = { 0.0f, 0.0f };
        glm::vec2 Size = { 0.5f, 0.5f };

        // TODO(Yan): move into physics material in the future maybe
        float Density = 1.0f;
        float Friction = 0.5f;
        float Restitution = 0.0f;
        float RestitutionThreshold = 0.5f;

        // Storage for runtime
        void* RuntimeFixture = nullptr;

        BoxCollider2DComponent() = default;
        BoxCollider2DComponent(const BoxCollider2DComponent&) = default;
    };

    struct CircleCollider2DComponent {
        glm::vec2 Offset = { 0.0f, 0.0f };
        float Radius = 0.5f;

        // TODO(Yan): move into physics material in the future maybe
        float Density = 1.0f;
        float Friction = 0.5f;
        float Restitution = 0.0f;
        float RestitutionThreshold = 0.5f;

        // Storage for runtime
        void* RuntimeFixture = nullptr;

        CircleCollider2DComponent() = default;
        CircleCollider2DComponent(const CircleCollider2DComponent&) = default;
    };
}


#endif //GAME_COMPONENTS_H
