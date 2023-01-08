// @author: lx
// @time: 2022/10/28
// @brief: 

#ifndef GAME_SCENEHIERARCHYPANEL_H
#define GAME_SCENEHIERARCHYPANEL_H

#include "Core/Core.h"
#include "Core/Log.h"
#include "Scene/Scene.h"
#include "Scene/Entity.h"
#include "glm/gtc/type_ptr.hpp"
#include <filesystem>


namespace Asura {
    class SceneHierarchyPanel {
    public:
        SceneHierarchyPanel() = default;
        SceneHierarchyPanel(const Ref<Scene>& context);

        void setContext(const Ref<Scene>& context);

        void onImGuiRender();

        Entity getSelectedEntity() const { return m_selectionContext; }

        void setSelectedEntity(Entity entity) {
            m_selectionContext = entity;
        }

    private:
        void drawEntityNode(Entity entity);
        void drawComponents(Entity entity);
        static void drawVec3Control(const std::string& label, glm::vec3& values,
                                    float resetValue = 0.f, float columnWidth = 100.f);

        template<typename T, typename UIFunction>
        static void drawComponent(const std::string& name, Entity entity, UIFunction uiFunction);

        Ref<Scene> m_context;
        Entity m_selectionContext;
    };
}


#endif //GAME_SCENEHIERARCHYPANEL_H
