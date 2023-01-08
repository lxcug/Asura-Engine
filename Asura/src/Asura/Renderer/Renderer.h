// @author: lx
// @time: 2022/10/15
// @brief: 

#pragma once
#ifndef GAME_RENDERER_H
#define GAME_RENDERER_H

#include "Asura/Core/Core.h"
#include "Renderer/RendererAPI.h"
#include "Renderer/Shader.h"
#include "OrthographicCamera.h"


namespace Asura {
    class ASURA_API Renderer {
    public:
        static void init();

        static void shutDown();

        static void beginScene(OrthographicCamera& orthographicCamera);
        static void endScene();

        static void submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray,
                           const glm::mat4& transform = glm::mat4(1.f));

        inline static RendererAPI::API getAPI() {
            return RendererAPI::getAPI();
        };

        static void onWindowResize(unsigned width, unsigned height);

    private:
        struct SceneData {
            glm::mat4 ViewProjectionMatrix;
        };

        static SceneData* m_sceneData;
    };
}


#endif //GAME_RENDERER_H
