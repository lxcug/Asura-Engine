#include "Scene.h"

#include "glm/glm.hpp"
#include "Renderer/Renderer2D.h"
#include "Scene/Components.h"
#include "Scene/Entity.h"
#include "Scene/ScriptableEntity.h"


namespace Asura {
    static b2BodyType rigidbody2DTypeToBox2DBody(Rigidbody2DComponent::BodyType bodyType) {
        switch (bodyType) {
            case Rigidbody2DComponent::BodyType::Static:    return b2_staticBody;
            case Rigidbody2DComponent::BodyType::Dynamic:   return b2_dynamicBody;
            case Rigidbody2DComponent::BodyType::Kinematic: return b2_kinematicBody;
        }

        ASURA_CORE_ASSERT(false, "Unknown body type");
        return b2_staticBody;
    }

    template<typename Component>
    static void copyComponent(entt::registry& dst, entt::registry& src, const std::unordered_map<GUID, entt::entity>& enttMap) {
        auto view = src.view<Component>();
        for(auto e : view) {
            GUID GUID = src.get<GUIDComponent>(e).ID;
            ASURA_CORE_ASSERT(enttMap.find(GUID) != enttMap.end(), "");
            entt::entity dstEnttID = enttMap.at(GUID);

            auto& component = src.get<Component>(e);
            dst.emplace_or_replace<Component>(dstEnttID, component);
        }
    }

    template<typename Component>
    static void copyComponentIfExists(Entity dst, Entity src) {
        if (src.hasComponent<Component>())
            dst.addOrReplaceComponent<Component>(src.getComponent<Component>());
    }
}


namespace Asura {
    Scene::Scene() {
        m_texture = Asura::Texture2D::createTexture2D("E:/Asura/Asura-Editor/assets/textures/dog.png");
    }

    Scene::~Scene() {
        delete m_physicsWorld;
    }

    void Scene::onUpdateRuntime(Timestep timestep) {
        // update scripts
        m_registry.view<NativeScriptComponent>().each([=](entt::entity entity, NativeScriptComponent& nsc) {
            // ScriptableEntity is created and deleted in every frame
            nsc.instance = nsc.instantiateScript();
            nsc.instance->m_entity = {entity, this};
            nsc.instance->onUpdate(timestep);
            nsc.destroyScript(&nsc);
        });

        const int32_t velocityIterations = 6;
        const int32_t positionIterations = 2;
        m_physicsWorld->Step(timestep, velocityIterations, positionIterations);

        auto view = m_registry.view<Rigidbody2DComponent>();
        for(auto e : view) {
            Entity entity = { e, this };
            auto& transform = entity.getComponent<TransformComponent>();
            auto& rb2d = entity.getComponent<Rigidbody2DComponent>();

            b2Body* body = (b2Body*)rb2d.RuntimeBody;
            const auto& position = body->GetPosition();
            transform.Translation.x = position.x;
            transform.Translation.y = position.y;
            transform.Rotation.z = body->GetAngle();
        }

        // switch camera
        Camera* mainCamera = nullptr;
        glm::mat4 cameraTransform; {
            auto view = m_registry.view<TransformComponent, CameraComponent>();
            for(auto entity : view) {
                auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

                if(camera.primary) {
                    mainCamera = &camera.Camera;
                    cameraTransform = transform.getTransform();
                    break;
                }
            }
        }

        if(mainCamera) {
            Renderer2D::beginScene(*mainCamera, cameraTransform);

            // draw quads
            {
                auto view = m_registry.view<TransformComponent, SpriteRendererComponent>();
                for(auto entity : view) {
                    auto [transform, sprite] =
                            view.get<TransformComponent, SpriteRendererComponent>(entity);

                    Renderer2D::drawSprite(transform.getTransform(), sprite, (int)entity);
                }
            }

            // draw circles
            {
                auto view = m_registry.view<TransformComponent, CircleRendererComponent>();
                for(auto entity : view) {
                    auto [transform, circle] =
                            view.get<TransformComponent, CircleRendererComponent>(entity);

                    Renderer2D::drawCircle(transform.getTransform(), circle.Color,
                                           circle.Thickness, circle.Fade, (int)entity);
                }
            }

            Renderer2D::endScene();
        }
    }

    void Scene::onUpdateEditor(Timestep timestep, EditorCamera& camera) {
        renderScene(camera);
    }

    void Scene::onRuntimeStart() {
        onPhysics2DStart();
    }

    void Scene::onRuntimeStop() {
        onPhysics2DStop();
    }

    void Scene::onViewportResize(unsigned int width, unsigned int height) {
        m_viewportWidth = width;
        m_viewportHeight = height;

        auto view = m_registry.view<CameraComponent>();
        for(auto entity : view) {
            auto& cameraComponent = view.get<CameraComponent>(entity);
            if(!cameraComponent.fixedAspectRatio) {
                cameraComponent.Camera.setViewportSize(width, height);
            }
        }
    }

    Entity Scene::createEntity(const std::string &name) {
        return createEntityWithGUID(GUID(), name);
    }

    Entity Scene::createEntityWithGUID(GUID guid, const std::string &name) {
        Entity entity = { m_registry.create(), this };
        entity.addComponent<GUIDComponent>(guid);
        entity.addComponent<TransformComponent>();
        auto& tag = entity.addComponent<TagComponent>();
        tag.Tag = name.empty() ? "Entity" : name;
        return entity;
    }

    void Scene::destroyEntity(Entity entity) {
        m_registry.destroy(entity);
    }

    Entity Scene::getPrimaryCameraEntity() {
        auto view = m_registry.view<CameraComponent>();
        for(auto entity : view) {
            const auto& camera = view.get<CameraComponent>(entity);
            if (camera.primary)
                return Entity{entity, this};
        }
        return {};
    }

    Ref<Scene> Scene::copy(Ref<Scene> other) {
        Ref<Scene> newScene = createRef<Scene>();

        newScene->m_viewportWidth = other->m_viewportWidth;
        newScene->m_viewportHeight = other->m_viewportHeight;

        auto& srcSceneRegistry = other->m_registry;
        auto& dstSceneRegistry = newScene->m_registry;
        std::unordered_map<GUID, entt::entity> enttMap;

        // Create entities in new scene
        auto idView = srcSceneRegistry.view<GUIDComponent>();
        for(auto e : idView) {
            GUID GUID = srcSceneRegistry.get<GUIDComponent>(e).ID;
            const auto& name = srcSceneRegistry.get<TagComponent>(e).Tag;
            Entity newEntity = newScene->createEntityWithGUID(GUID, name);
            enttMap[GUID] = (entt::entity)newEntity;
        }

        copyComponent<TransformComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
        copyComponent<SpriteRendererComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
        copyComponent<CircleRendererComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
        copyComponent<CameraComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
        copyComponent<NativeScriptComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
        copyComponent<Rigidbody2DComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
        copyComponent<BoxCollider2DComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
        copyComponent<CircleCollider2DComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
        return newScene;
    }

    void Scene::duplicateEntity(Entity entity) {
        std::string name = entity.getName();
        Entity newEntity = createEntity(name);

        copyComponentIfExists<TransformComponent>(newEntity, entity);
        copyComponentIfExists<SpriteRendererComponent>(newEntity, entity);
        copyComponentIfExists<CameraComponent>(newEntity, entity);
        copyComponentIfExists<NativeScriptComponent>(newEntity, entity);
        copyComponentIfExists<Rigidbody2DComponent>(newEntity, entity);
        copyComponentIfExists<BoxCollider2DComponent>(newEntity, entity);
        copyComponentIfExists<CircleCollider2DComponent>(newEntity, entity);
    }

    void Scene::onUpdateSimulation(Timestep ts, EditorCamera &camera) {
        {
            const int32_t velocityIterations = 6;
            const int32_t positionIterations = 2;
            m_physicsWorld->Step(ts, velocityIterations, positionIterations);

            // Retrieve transform from Box2D
            auto view = m_registry.view<Rigidbody2DComponent>();
            for(auto e : view) {
                Entity entity = { e, this };
                auto& transform = entity.getComponent<TransformComponent>();
                auto& rb2d = entity.getComponent<Rigidbody2DComponent>();
                b2Body* body = (b2Body*)rb2d.RuntimeBody;
                const auto& position = body->GetPosition();
                transform.Translation.x = position.x;
                transform.Translation.y = position.y;
                transform.Rotation.z = body->GetAngle();
            }
        }
        renderScene(camera);
    }

    void Scene::onSimulationStart() {
        onPhysics2DStart();
    }

    void Scene::onSimulationStop() {
        onPhysics2DStop();
    }

    void Scene::onPhysics2DStart() {
        m_physicsWorld = new b2World({ 0.0f, -9.8f });

        auto view = m_registry.view<Rigidbody2DComponent>();
        for(auto e : view) {
            Entity entity = { e, this };
            auto& transform = entity.getComponent<TransformComponent>();
            auto& rb2d = entity.getComponent<Rigidbody2DComponent>();

            b2BodyDef bodyDef;
            bodyDef.type = rigidbody2DTypeToBox2DBody(rb2d.Type);
            bodyDef.position.Set(transform.Translation.x, transform.Translation.y);
            bodyDef.angle = transform.Rotation.z;

            b2Body* body = m_physicsWorld->CreateBody(&bodyDef);
            body->SetFixedRotation(rb2d.FixedRotation);
            rb2d.RuntimeBody = body;

            if(entity.hasComponent<BoxCollider2DComponent>()) {
                auto& bc2d = entity.getComponent<BoxCollider2DComponent>();

                b2PolygonShape boxShape;
                boxShape.SetAsBox(bc2d.Size.x * transform.Scale.x, bc2d.Size.y * transform.Scale.y);

                b2FixtureDef fixtureDef;
                fixtureDef.shape = &boxShape;
                fixtureDef.density = bc2d.Density;
                fixtureDef.friction = bc2d.Friction;
                fixtureDef.restitution = bc2d.Restitution;
                fixtureDef.restitutionThreshold = bc2d.RestitutionThreshold;
                body->CreateFixture(&fixtureDef);
            }

            if(entity.hasComponent<CircleCollider2DComponent>()) {
                auto& cc2d = entity.getComponent<CircleCollider2DComponent>();

                b2CircleShape circleShape;
                circleShape.m_p.Set(cc2d.Offset.x, cc2d.Offset.y);
                circleShape.m_radius = transform.Scale.x * cc2d.Radius;

                b2FixtureDef fixtureDef;
                fixtureDef.shape = &circleShape;
                fixtureDef.density = cc2d.Density;
                fixtureDef.friction = cc2d.Friction;
                fixtureDef.restitution = cc2d.Restitution;
                fixtureDef.restitutionThreshold = cc2d.RestitutionThreshold;
                body->CreateFixture(&fixtureDef);
            }
        }
    }

    void Scene::onPhysics2DStop() {
        delete m_physicsWorld;
        m_physicsWorld = nullptr;
    }

    void Scene::renderScene(EditorCamera& camera) {
        Renderer2D::beginScene(camera);

        // draw quads
        {
            auto view = m_registry.view<TransformComponent, SpriteRendererComponent>();
            for(auto entity : view) {
                auto [transform, sprite] =
                        view.get<TransformComponent, SpriteRendererComponent>(entity);

                Renderer2D::drawSprite(transform.getTransform(), sprite, (int)entity);
            }
        }

        // draw circles
        {
            auto view = m_registry.view<TransformComponent, CircleRendererComponent>();
            for(auto entity : view) {
                auto [transform, circle] =
                        view.get<TransformComponent, CircleRendererComponent>(entity);

                Renderer2D::drawCircle(transform.getTransform(), circle.Color,
                                       circle.Thickness, circle.Fade, (int)entity);
            }
        }

        Renderer2D::endScene();
    }

    template<typename T>
    void Scene::onComponentAdded(Entity entity, T &component) {

    }

    template<>
    void Scene::onComponentAdded(Entity entity, TagComponent &component) {
    }

    template<>
    void Scene::onComponentAdded(Entity entity, TransformComponent &component) {
    }

    template<>
    void Scene::onComponentAdded(Entity entity, SpriteRendererComponent &component) {
    }

    template<>
    void Scene::onComponentAdded<CircleRendererComponent>(Entity entity, CircleRendererComponent& component) {
    }

    template<>
    void Scene::onComponentAdded(Entity entity, CameraComponent &component) {
//        if(m_viewportWidth > 0 && m_viewportHeight > 0)
//            component.Camera.setViewportSize(m_viewportWidth, m_viewportHeight);
    }
    template<>
    void Scene::onComponentAdded(Entity entity, NativeScriptComponent &component) {
    }

    template<>
    void Scene::onComponentAdded<Rigidbody2DComponent>(Entity entity, Rigidbody2DComponent& component) {
    }

    template<>
    void Scene::onComponentAdded<BoxCollider2DComponent>(Entity entity, BoxCollider2DComponent& component) {
    }

    template<>
    void Scene::onComponentAdded<GUIDComponent>(Entity entity, GUIDComponent& component) {
    }

    template<>
    void Scene::onComponentAdded<CircleCollider2DComponent>(Entity entity, CircleCollider2DComponent& component) {
    }
}

