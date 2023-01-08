// @author: lx
// @time: 2022/10/28
// @brief: 

#ifndef GAME_SCRIPTABLEENTITY_H
#define GAME_SCRIPTABLEENTITY_H

#include "Entity.h"


namespace Asura {
    class ScriptableEntity {
    public:
        friend class Scene;

        template<typename T>
        T& getComponent() {
            return m_entity.getComponent<T>();
        }

        template<typename T>
        bool hasComponent() const {
            return m_entity.hasComponent<T>();
        }

        virtual void onCreate() {}
        virtual void onDestroy() {}
        virtual void onUpdate(Timestep timestep) {}

    protected:
        Entity m_entity;
    };
}


#endif //GAME_SCRIPTABLEENTITY_H
