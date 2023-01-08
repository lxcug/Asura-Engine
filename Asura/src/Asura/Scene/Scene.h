// @author: lx
// @time: 2022/10/26
// @brief: 

#ifndef GAME_SCENE_H
#define GAME_SCENE_H

#include "Core/Core.h"
#include "entt.hpp"
#include "Core/Timestep.h"
#include "Renderer/EditorCamera.h"
#include "Renderer/Texture.h"
#include "Core/GUID.h"

#include "box2d/b2_world.h"
#include "box2d/b2_body.h"
#include "box2d/b2_fixture.h"
#include "box2d/b2_polygon_shape.h"
#include "box2d/b2_circle_shape.h"


namespace Asura {
    class Entity;
    class Scene {
    public:
        friend class Entity;
        friend class SceneHierarchyPanel;
        friend class SceneSerializer;

        Scene();
        ~Scene();

        static Ref<Scene> copy(Ref<Scene> other);
        void duplicateEntity(Entity entity);

        entt::registry& reg() {
            return m_registry;
        }

        void onUpdateRuntime(Timestep timestep);
        void onUpdateEditor(Timestep timestep, EditorCamera& camera);
        void onUpdateSimulation(Timestep timestep, EditorCamera& camera);

        void onRuntimeStart();
        void onRuntimeStop();

        void onSimulationStart();
        void onSimulationStop();

        Entity createEntity(const std::string& name = std::string());
        Entity createEntityWithGUID(GUID guid, const std::string& name = std::string());

        void destroyEntity(Entity entity);

        void onViewportResize(unsigned width, unsigned height);

        Entity getPrimaryCameraEntity();

        template<typename... Components>
        auto getAllEntitiesWith() {
            return m_registry.view<Components...>();
        }

        entt::registry& getRegistry() {
            return m_registry;
        }

    private:
        void onPhysics2DStart();
        void onPhysics2DStop();

        template<typename T>
        void onComponentAdded(Entity entity, T& component);

        void renderScene(EditorCamera& camera);

    private:
        entt::registry m_registry;
        unsigned m_viewportWidth = 0, m_viewportHeight = 0;

        Ref<Texture2D> m_texture;

        b2World* m_physicsWorld = nullptr;
    };
}


#endif //GAME_SCENE_H
