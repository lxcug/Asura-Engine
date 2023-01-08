// @author: lx
// @time: 2022/10/29
// @brief: 

#ifndef GAME_SCENESERIALIZER_H
#define GAME_SCENESERIALIZER_H

#include "Core/Core.h"
#include "Scene/Scene.h"
#include "glm/glm.hpp"
#include "yaml-cpp/yaml.h"


namespace Asura {
    class SceneSerializer {
    public:
        SceneSerializer(const Ref<Scene>& scene): m_scene(scene) {}

        void serialize(const std::string& filePath);
        void serializeRuntime(const std::string& filePath);

        bool deserialize(const std::string& filePath);
        bool deserializeRuntime(const std::string& filePath);

    private:
        Ref<Scene> m_scene;
    };

    static void serializeEntity(YAML::Emitter& out, Entity entity);
}


#endif //GAME_SCENESERIALIZER_H
