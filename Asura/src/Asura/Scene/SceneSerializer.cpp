#include "SceneSerializer.h"
#include "Scene/Entity.h"
#include "Scene/Components.h"
#include "Core/Log.h"

#include <fstream>


namespace YAML {
    template<>
    struct convert<glm::vec2> {
        static Node encode(const glm::vec2& rhs) {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.SetStyle(EmitterStyle::Flow);
            return node;
        }

        static bool decode(const Node& node, glm::vec2& rhs) {
            if (!node.IsSequence() || node.size() != 2)
                return false;

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            return true;
        }
    };

    template<>
    struct convert<glm::vec3> {
        static Node encode(const glm::vec3& rhs) {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            return node;
        }

        static bool decode(const Node& node, glm::vec3& rhs) {
            if(!node.IsSequence() || node.size() != 3)
                return false;

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            rhs.z = node[2].as<float>();
            return true;
        }
    };

    template<>
    struct convert<glm::vec4> {
        static Node encode(const glm::vec4& rhs) {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            node.push_back(rhs.w);
            return node;
        }

        static bool decode(const Node& node, glm::vec4& rhs) {
            if(!node.IsSequence() || node.size() != 4)
                return false;

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            rhs.z = node[2].as<float>();
            rhs.w = node[3].as<float>();
            return true;
        }
    };
}


namespace Asura {
    YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& vec) {
        out << YAML::Flow;
        out << YAML::BeginSeq << vec.x << vec.y << YAML::EndSeq;
        return out;
    }

    YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& vec) {
        out << YAML::Flow;
        out << YAML::BeginSeq << vec.x << vec.y << vec.z << YAML::EndSeq;
        return out;
    }

    YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& vec) {
        out << YAML::Flow;
        out << YAML::BeginSeq << vec.x << vec.y << vec.z << vec.w << YAML::EndSeq;
        return out;
    }

    static std::string rigidBody2DBodyTypeToString(Rigidbody2DComponent::BodyType bodyType) {
        switch (bodyType) {
            case Rigidbody2DComponent::BodyType::Static:    return "Static";
            case Rigidbody2DComponent::BodyType::Dynamic:   return "Dynamic";
            case Rigidbody2DComponent::BodyType::Kinematic: return "Kinematic";
        }
        ASURA_CORE_ASSERT(false, "Unknown body type");
        return {};
    }


    static Rigidbody2DComponent::BodyType rigidBody2DBodyTypeFromString(const std::string& bodyTypeString) {
        if (bodyTypeString == "Static")    return Rigidbody2DComponent::BodyType::Static;
        if (bodyTypeString == "Dynamic")   return Rigidbody2DComponent::BodyType::Dynamic;
        if (bodyTypeString == "Kinematic") return Rigidbody2DComponent::BodyType::Kinematic;
        ASURA_CORE_ASSERT(false, "Unknown body type");
        return Rigidbody2DComponent::BodyType::Static;
    }
} 


void Asura::SceneSerializer::serialize(const std::string &filePath) {
    YAML::Emitter out;
    out << YAML::BeginMap;
    out << YAML::Key << "Scene" << YAML::Value << "Untitled";
    out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
    m_scene->m_registry.each([&](auto entityID){
        Entity entity = { entityID, m_scene.get() };
        if (!entity)
            return;
        serializeEntity(out, entity);
    });
    out << YAML::EndSeq;
    out << YAML::EndMap;

    std::ofstream fout(filePath);
    fout << out.c_str();
}

void Asura::SceneSerializer::serializeRuntime(const std::string &filePath) {
    // todo
}

bool Asura::SceneSerializer::deserialize(const std::string &filePath) {
    YAML::Node data;
    try {
        data = YAML::LoadFile(filePath);
    }
    catch (YAML::ParserException& e) {
        ASURA_CORE_ERROR("Failed to load .asura file '{0}'\n     {1}", filePath, e.what());
        return false;
    }

    if(!data["Scene"]) {
        return false;
    }

    auto sceneName = data["Scene"].as<std::string>();
    ASURA_CORE_TRACE("Deserializing scene '{0}'", sceneName);

    auto entities = data["Entities"];
    if(!entities)
        return false;
    for(auto entity : entities) {
        auto guid = entity["Entity"].as<uint64_t>();
        std::string name;
        auto tagComponent = entity["TagComponent"];
        if(tagComponent)
            name = tagComponent["Tag"].as<std::string>();
        ASURA_CORE_TRACE("Deserialized entity with ID = {0}, name = {1}", guid, name);

        Entity deserializedEntity = m_scene->createEntityWithGUID(guid, name);
        auto transformComponent = entity["TransformComponent"];
        if(transformComponent) {
            auto& tc = deserializedEntity.getComponent<TransformComponent>();
            tc.Translation = transformComponent["Translation"].as<glm::vec3>();
            tc.Rotation = transformComponent["Rotation"].as<glm::vec3>();
            tc.Scale = transformComponent["Scale"].as<glm::vec3>();
        }

        auto spriteRendererComponent = entity["SpriteRendererComponent"];
        if(spriteRendererComponent) {
            auto& src = deserializedEntity.addComponent<SpriteRendererComponent>();
            src.Color = spriteRendererComponent["Color"].as<glm::vec4>();
            if(spriteRendererComponent["TexturePath"])
                src.Texture = Texture2D::createTexture2D(spriteRendererComponent["TexturePath"].as<std::string>());
            if(spriteRendererComponent["TilingFactor"])
                src.TilingFactor = spriteRendererComponent["TilingFactor"].as<float>();
        }

        auto circleRendererComponent = entity["CircleRendererComponent"];
        if(circleRendererComponent) {
            auto& src = deserializedEntity.addComponent<CircleRendererComponent>();
            src.Color = circleRendererComponent["Color"].as<glm::vec4>();
            src.Thickness = circleRendererComponent["Thickness"].as<float>();
            src.Fade = circleRendererComponent["Fade"].as<float>();
        }

        auto cameraComponent = entity["CameraComponent"];
        if(cameraComponent) {
            auto& cc = deserializedEntity.addComponent<CameraComponent>();
            auto cameraProps = cameraComponent["Camera"];
            cc.Camera.setPerspectiveVerticalFOV(cameraProps["PerspectiveFOV"].as<float>());
            cc.Camera.setPerspectiveNearClip(cameraProps["PerspectiveNear"].as<float>());
            cc.Camera.setPerspectiveFarClip(cameraProps["PerspectiveFar"].as<float>());

            cc.Camera.setOrthographicSize(cameraProps["OrthographicSize"].as<float>());
            cc.Camera.setOrthographicNearClip(cameraProps["OrthographicNear"].as<float>());
            cc.Camera.setOrthographicFarClip(cameraProps["OrthographicFar"].as<float>());

            cc.primary = cameraComponent["Primary"].as<bool>();
            cc.fixedAspectRatio = cameraComponent["FixedAspectRatio"].as<bool>();
            cc.Camera.setProjectionType((SceneCamera::ProjectionType)cameraProps["ProjectionType"].as<int>());
        }

        auto rigidbody2DComponent = entity["Rigidbody2DComponent"];
        if(rigidbody2DComponent) {
            auto& rb2d = deserializedEntity.addComponent<Rigidbody2DComponent>();
            rb2d.Type = rigidBody2DBodyTypeFromString(rigidbody2DComponent["BodyType"].as<std::string>());
            rb2d.FixedRotation = rigidbody2DComponent["FixedRotation"].as<bool>();
        }

        auto boxCollider2DComponent = entity["BoxCollider2DComponent"];
        if(boxCollider2DComponent) {
            auto& bc2d = deserializedEntity.addComponent<BoxCollider2DComponent>();
            bc2d.Offset = boxCollider2DComponent["Offset"].as<glm::vec2>();
            bc2d.Size = boxCollider2DComponent["Size"].as<glm::vec2>();
            bc2d.Density = boxCollider2DComponent["Density"].as<float>();
            bc2d.Friction = boxCollider2DComponent["Friction"].as<float>();
            bc2d.Restitution = boxCollider2DComponent["Restitution"].as<float>();
            bc2d.RestitutionThreshold = boxCollider2DComponent["RestitutionThreshold"].as<float>();
        }

        auto circleCollider2DComponent = entity["CircleCollider2DComponent"];
        if(circleCollider2DComponent) {
            auto& cc2d = deserializedEntity.addComponent<CircleCollider2DComponent>();
            cc2d.Offset = circleCollider2DComponent["Offset"].as<glm::vec2>();
            cc2d.Radius = circleCollider2DComponent["Radius"].as<float>();
            cc2d.Density = circleCollider2DComponent["Density"].as<float>();
            cc2d.Friction = circleCollider2DComponent["Friction"].as<float>();
            cc2d.Restitution = circleCollider2DComponent["Restitution"].as<float>();
            cc2d.RestitutionThreshold = circleCollider2DComponent["RestitutionThreshold"].as<float>();
        }
    }
    return true;
}

bool Asura::SceneSerializer::deserializeRuntime(const std::string &filePath) {
    // todo
    return false;
}

void Asura::serializeEntity(YAML::Emitter &out, Asura::Entity entity) {
    ASURA_CORE_ASSERT(entity.hasComponent<GUIDComponent>(), "Entity with no GUID");

    out << YAML::BeginMap;  // Entity
    out << YAML::Key << "Entity" << YAML::Value << entity.getGUID(); // TODO: Entity ID goes here

    if(entity.hasComponent<TagComponent>()) {
        out << YAML::Key << "TagComponent";
        out << YAML::BeginMap;  // TagComponent

        auto& tag = entity.getComponent<TagComponent>().Tag;
        out << YAML::Key << "Tag" << YAML::Value << tag;

        out << YAML::EndMap;  // TagComponent
    }

    if(entity.hasComponent<TransformComponent>()) {
        out << YAML::Key << "TransformComponent";
        out << YAML::BeginMap;  // TransformComponent

        auto& tc = entity.getComponent<TransformComponent>();
        out << YAML::Key << "Translation" << YAML::Value << tc.Translation;
        out << YAML::Key << "Rotation" << YAML::Value << tc.Rotation;
        out << YAML::Key << "Scale" << YAML::Value << tc.Scale;

        out << YAML::EndMap;  // TransformComponent
    }

    if(entity.hasComponent<SpriteRendererComponent>()) {
        out << YAML::Key << "SpriteRendererComponent";
        out << YAML::BeginMap;  // SpriteRendererComponent

        auto& spriteRendererComponent = entity.getComponent<SpriteRendererComponent>();
        out << YAML::Key << "Color" << YAML::Value << spriteRendererComponent.Color;
        if (spriteRendererComponent.Texture)
            out << YAML::Key << "TexturePath" << YAML::Value << spriteRendererComponent.Texture->getPath();
        out << YAML::Key << "TilingFactor" << YAML::Value << spriteRendererComponent.TilingFactor;
        out << YAML::EndMap;  // SpriteRendererComponent
    }

    if(entity.hasComponent<CircleRendererComponent>()) {
        out << YAML::Key << "CircleRendererComponent";
        out << YAML::BeginMap;  // CircleRendererComponent

        auto& circleRendererComponent = entity.getComponent<CircleRendererComponent>();
        out << YAML::Key << "Color" << YAML::Value << circleRendererComponent.Color;
        out << YAML::Key << "Thickness" << YAML::Value << circleRendererComponent.Thickness;
        out << YAML::Key << "Fade" << YAML::Value << circleRendererComponent.Fade;
        out << YAML::EndMap;  // CircleRendererComponent
    }

    if(entity.hasComponent<CameraComponent>()) {
        out << YAML::Key << "CameraComponent";
        out << YAML::BeginMap;  // CameraComponent

        auto& cameraComponent = entity.getComponent<CameraComponent>();
        auto& camera = cameraComponent.Camera;

        out << YAML::Key << "Camera" << YAML::Value;
        out << YAML::BeginMap;  // Camera
        out << YAML::Key << "ProjectionType" << YAML::Value << (int)camera.getProjectionType();
        out << YAML::Key << "PerspectiveFOV" << YAML::Value << camera.getPerspectiveVerticalFOV();
        out << YAML::Key << "PerspectiveNear" << YAML::Value << camera.getPerspectiveNearClip();
        out << YAML::Key << "PerspectiveFar" << YAML::Value << camera.getPerspectiveFarClip();
        out << YAML::Key << "OrthographicSize" << YAML::Value << camera.getOrthographicSize();
        out << YAML::Key << "OrthographicNear" << YAML::Value << camera.getOrthographicNearClip();
        out << YAML::Key << "OrthographicFar" << YAML::Value << camera.getOrthographicFarClip();
        out << YAML::EndMap; // Camera

        out << YAML::Key << "Primary" << YAML::Value << cameraComponent.primary;
        out << YAML::Key << "FixedAspectRatio" << YAML::Value << cameraComponent.fixedAspectRatio;

        out << YAML::EndMap;  // CameraComponent
    }

    if(entity.hasComponent<Rigidbody2DComponent>()) {
        out << YAML::Key << "Rigidbody2DComponent";
        out << YAML::BeginMap; // Rigidbody2DComponent

        auto& rb2dComponent = entity.getComponent<Rigidbody2DComponent>();
        out << YAML::Key << "BodyType" << YAML::Value << rigidBody2DBodyTypeToString(rb2dComponent.Type);
        out << YAML::Key << "FixedRotation" << YAML::Value << rb2dComponent.FixedRotation;

        out << YAML::EndMap; // Rigidbody2DComponent
    }

    if(entity.hasComponent<BoxCollider2DComponent>()) {
        out << YAML::Key << "BoxCollider2DComponent";
        out << YAML::BeginMap; // BoxCollider2DComponent

        auto& bc2dComponent = entity.getComponent<BoxCollider2DComponent>();
        out << YAML::Key << "Offset" << YAML::Value << bc2dComponent.Offset;
        out << YAML::Key << "Size" << YAML::Value << bc2dComponent.Size;
        out << YAML::Key << "Density" << YAML::Value << bc2dComponent.Density;
        out << YAML::Key << "Friction" << YAML::Value << bc2dComponent.Friction;
        out << YAML::Key << "Restitution" << YAML::Value << bc2dComponent.Restitution;
        out << YAML::Key << "RestitutionThreshold" << YAML::Value << bc2dComponent.RestitutionThreshold;
        out << YAML::EndMap; // BoxCollider2DComponent
    }

    if(entity.hasComponent<CircleCollider2DComponent>()) {
        out << YAML::Key << "CircleCollider2DComponent";
        out << YAML::BeginMap; // CircleCollider2DComponent

        auto& cc2dComponent = entity.getComponent<CircleCollider2DComponent>();
        out << YAML::Key << "Offset" << YAML::Value << cc2dComponent.Offset;
        out << YAML::Key << "Radius" << YAML::Value << cc2dComponent.Radius;
        out << YAML::Key << "Density" << YAML::Value << cc2dComponent.Density;
        out << YAML::Key << "Friction" << YAML::Value << cc2dComponent.Friction;
        out << YAML::Key << "Restitution" << YAML::Value << cc2dComponent.Restitution;
        out << YAML::Key << "RestitutionThreshold" << YAML::Value << cc2dComponent.RestitutionThreshold;

        out << YAML::EndMap; // CircleCollider2DComponent
    }

    out << YAML::EndMap;  // Entity
}
