// @author: lx
// @time: 2022/10/19
// @brief: 

#pragma once
#ifndef GAME_RENDERER2D_H
#define GAME_RENDERER2D_H

#include "Asura/Core/Core.h"
#include "OrthographicCamera.h"
#include "Renderer/VertexArray.h"
#include "Renderer/Shader.h"
#include "Renderer/Texture.h"
#include "Renderer/Camera.h"
#include "Renderer/EditorCamera.h"
#include "Scene/Components.h"
#include "glm/gtc/type_ptr.hpp"
#include "Renderer/UniformBuffer.h"


namespace Asura {
    struct QuadVertex {
        glm::vec3 Position;
        glm::vec4 Color;
        glm::vec2 TexCoord;
        float TexIndex;
        float TilingFactor;
        int EntityID;
    };

    struct LineVertex {
        glm::vec3 Position;
        glm::vec4 Color;
        int EntityID;
    };

    struct CircleVertex {
        glm::vec3 WorldPosition;
        glm::vec3 LocalPosition;
        glm::vec4 Color;
        float Thickness;
        float Fade;
        int EntityID;
    };

    struct Statistics {
        unsigned DrawCalls = 0;
        unsigned QuadCount = 0;

        unsigned getTotalVertexCount() {
            return QuadCount * 4;
        }
        unsigned getTotalIndexCount() {
            return QuadCount * 6;
        }
    };

    struct CameraData {
        glm::mat4 ViewProjection;
    };

    struct Renderer2DData {
        static const unsigned MaxQuads = 10000;
        static const unsigned MaxVertices = MaxQuads * 4;
        static const unsigned MaxIndices = MaxQuads * 6;
        static const unsigned MaxTextureSlots = 32;

        // for quad
        Ref<VertexArray> QuadVertexArray;
        Ref<VertexBuffer> QuadVertexBuffer;
        Ref<Shader> QuadShader;
        uint32_t QuadIndexCount = 0;
        QuadVertex* QuadVertexBufferBase = nullptr;
        QuadVertex* QuadVertexBufferPtr = nullptr;
        glm::vec4 QuadVertexPositions[4];

        // for line
        Ref<VertexArray> LineVertexArray;
        Ref<VertexBuffer> LineVertexBuffer;
        Ref<Shader> LineShader;
        float LineWidth = 2.0f;
        uint32_t LineVertexCount = 0;
        LineVertex* LineVertexBufferBase = nullptr;
        LineVertex* LineVertexBufferPtr = nullptr;

        // for circle
        Ref<VertexArray> CircleVertexArray;
        Ref<VertexBuffer> CircleVertexBuffer;
        Ref<Shader> CircleShader;
        uint32_t CircleIndexCount = 0;
        CircleVertex* CircleVertexBufferBase = nullptr;
        CircleVertex* CircleVertexBufferPtr = nullptr;

        Ref<Texture2D> WhiteTexture;
        std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
        unsigned TextureSlotIndex = 1;  // 0 for white texture

        Statistics Stats;

        CameraData CameraBuffer;
        Ref<UniformBuffer> CameraUniformBuffer;
    };
}


namespace Asura {
    /**
     * @brief renderer API to draw quad
     */
    class ASURA_API Renderer2D {
    public:
        /**
         * @brief called by renderer to initialize renderer2D
         */
        static void init();
        /**
         * @brief called by renderer to shutdown renderer2D
         */
        static void shutDown();

        /**
         * @brief set camera viewport
         * @param camera orthographic camera
         */
        static void beginScene(const OrthographicCamera& camera);
        static void beginScene(const EditorCamera& camera);
        static void beginScene(const Camera& camera, const glm::mat4& transform);
        static void endScene();

        static void flush();

        static void drawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
        static void drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
        static void drawQuad(const glm::vec2& position, const glm::vec2& size, Ref<Asura::Texture2D>& texture,
                             float tilingFactor=1.f, const glm::vec4& tintColor = glm::vec4(1.f));
        static void drawQuad(const glm::vec3& position, const glm::vec2& size, Ref<Asura::Texture2D>& texture,
                             float tilingFactor=1.f, const glm::vec4& tintColor = glm::vec4(1.f));

        static void drawQuad(const glm::mat4& transform, const glm::vec4& color, int EntityID = -1);
        static void drawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, float tilingFactor = 1.0f,
                             const glm::vec4& tintColor = glm::vec4(1.0f), int EntityID = -1);

        static void drawRotateQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color);
        static void drawRotateQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color);
        static void drawRotateQuad(const glm::vec2& position, const glm::vec2& size, float rotation, Ref<Texture2D>& texture,
                                   float tilingFactor, const glm::vec4& tintColor = glm::vec4(1.f));
        static void drawRotateQuad(const glm::vec3& position, const glm::vec2& size, float rotation, Ref<Texture2D>& texture,
                                   float tilingFactor, const glm::vec4& tintColor = glm::vec4(1.f));

        static void drawSprite(const glm::mat4& transform, SpriteRendererComponent& src, int entityID);

        static void drawCircle(const glm::mat4& transform, const glm::vec4& color, float thickness = 1.0f,
                               float fade = 0.005f, int entityID = -1);

        static void drawLine(const glm::vec3& p0, const glm::vec3& p1, const glm::vec4& color, int entityID = -1);

        static void drawRect(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, int entityID = -1);
        static void drawRect(const glm::mat4& transform, const glm::vec4& color, int entityID = -1);

        static float getLineWidth() {
            return s_data.LineWidth;
        }
        static void setLineWidth(float width) {
            s_data.LineWidth = width;
        }

        static void resetStats() {
            memset(&s_data.Stats, 0, sizeof(s_data.Stats));
        }
        static Statistics getStats() {
            return s_data.Stats;
        }

    private:
        static void startBatch();
        static void nextBatch();

    private:
        static Renderer2DData s_data;
    };
}


#endif //GAME_RENDERER2D_H
