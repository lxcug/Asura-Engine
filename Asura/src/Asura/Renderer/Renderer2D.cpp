#include "Renderer2D.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "RenderCommand.h"


Asura::Renderer2DData Asura::Renderer2D::s_data;

void Asura::Renderer2D::init() {
    ASURA_PROFILE_FUNCTION();

    // quad index buffer
    auto* indices = new unsigned int [Asura::Renderer2DData::MaxIndices];
    unsigned offset = 0;
    for(int i = 0; i < Asura::Renderer2DData::MaxIndices; i += 6) {
        indices[i] = offset;
        indices[i+1] = offset + 1;
        indices[i+2] = offset + 2;

        indices[i+3] = offset + 2;
        indices[i+4] = offset + 3;
        indices[i+5] = offset;

        offset += 4;
    }
    Ref<IndexBuffer> quadIndexBuffer = Asura::IndexBuffer::createIndexBuffer(indices, Asura::Renderer2DData::MaxIndices);
    delete[] indices;

    // for quad
    s_data.QuadVertexArray = Asura::VertexArray::createVertexArray();
    s_data.QuadVertexBuffer = Asura::VertexBuffer::createVertexBuffer(Asura::Renderer2DData::MaxVertices * sizeof(QuadVertex));
    s_data.QuadVertexBuffer->setLayout({
        {ShaderDataType::Float3, "a_Position"},
        {ShaderDataType::Float4, "a_Color"},
        {ShaderDataType::Float2, "a_TexCoord"},
        {ShaderDataType::Float, "a_TexIndex"},
        {ShaderDataType::Float, "a_TilingFactor"},
        {ShaderDataType::Int, "a_EntityID"},
    });
    s_data.QuadVertexArray->addVertexBuffer(s_data.QuadVertexBuffer);
    s_data.QuadVertexArray->setIndexBuffer(quadIndexBuffer);
    s_data.QuadVertexBufferBase = new QuadVertex[Asura::Renderer2DData::MaxVertices];

    // for line
    s_data.LineVertexArray = VertexArray::createVertexArray();
    s_data.LineVertexBuffer = VertexBuffer::createVertexBuffer(Asura::Renderer2DData::MaxVertices * sizeof(LineVertex));
    s_data.LineVertexBuffer->setLayout({
        { ShaderDataType::Float3, "a_Position" },
        { ShaderDataType::Float4, "a_Color"    },
        { ShaderDataType::Int,    "a_EntityID" }
    });
    s_data.LineVertexArray->addVertexBuffer(s_data.LineVertexBuffer);
    s_data.LineVertexBufferBase = new LineVertex[Asura::Renderer2DData::MaxVertices];

    
    // for circle
    s_data.CircleVertexArray = VertexArray::createVertexArray();
    s_data.CircleVertexBuffer = VertexBuffer::createVertexBuffer(Asura::Renderer2DData::MaxVertices * sizeof(CircleVertex));
    s_data.CircleVertexBuffer->setLayout({
         { ShaderDataType::Float3, "a_WorldPosition" },
         { ShaderDataType::Float3, "a_LocalPosition" },
         { ShaderDataType::Float4, "a_Color"         },
         { ShaderDataType::Float,  "a_Thickness"     },
         { ShaderDataType::Float,  "a_Fade"          },
         { ShaderDataType::Int,    "a_EntityID"      }
     });
    s_data.CircleVertexArray->addVertexBuffer(s_data.CircleVertexBuffer);
    s_data.CircleVertexArray->setIndexBuffer(quadIndexBuffer);
    s_data.CircleVertexBufferBase = new CircleVertex[Asura::Renderer2DData::MaxVertices];


    s_data.WhiteTexture = Asura::Texture2D::createTexture2D(1, 1);
    unsigned whiteTextureData = 0xffffffff;
    s_data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

    int samplers[Asura::Renderer2DData::MaxTextureSlots];
    for(int i = 0; i < Asura::Renderer2DData::MaxTextureSlots; i++) {
        samplers[i] = i;
    }
    // set all texture slot to 0
    s_data.TextureSlots[0] = s_data.WhiteTexture;

    s_data.QuadShader = Asura::Shader::createShader("../../Asura-Editor/assets/shaders/Renderer2D_Quad.glsl");
    s_data.CircleShader = Shader::createShader("../../Asura-Editor/assets/shaders/Renderer2D_Circle.glsl");
    s_data.LineShader = Shader::createShader("../../Asura-Editor/assets/shaders/Renderer2D_Line.glsl");


    s_data.QuadVertexPositions[0] = {-.5f, -.5f, 0.f, 1.f};
    s_data.QuadVertexPositions[1] = {.5f, -.5f, 0.f, 1.f};
    s_data.QuadVertexPositions[2] = {.5f, .5f, 0.f, 1.f};
    s_data.QuadVertexPositions[3] = {-.5f, .5f, 0.f, 1.f};

    s_data.CameraUniformBuffer = UniformBuffer::createUniformBuffer(sizeof(CameraData), 0);
}

void Asura::Renderer2D::shutDown() {
    ASURA_PROFILE_FUNCTION();
    delete[] s_data.QuadVertexBufferBase;
    delete[] s_data.LineVertexBufferBase;
    delete[] s_data.CircleVertexBufferBase;
}

void Asura::Renderer2D::beginScene(const Asura::OrthographicCamera &camera) {
    ASURA_PROFILE_FUNCTION();

    s_data.CameraBuffer.ViewProjection = camera.getViewProjectionMatrix();
    s_data.CameraUniformBuffer->setData(&s_data.CameraBuffer, sizeof(CameraData));
    startBatch();
}

void Asura::Renderer2D::beginScene(const Asura::EditorCamera &camera) {
    ASURA_PROFILE_FUNCTION();
    s_data.CameraBuffer.ViewProjection = camera.getViewProjection();
    s_data.CameraUniformBuffer->setData(&s_data.CameraBuffer, sizeof(CameraData));
    startBatch();
}

void Asura::Renderer2D::beginScene(const Asura::Camera &camera, const glm::mat4 &transform) {
    ASURA_PROFILE_FUNCTION();
    s_data.CameraBuffer.ViewProjection = camera.getProjection() * glm::inverse(transform);
    s_data.CameraUniformBuffer->setData(&s_data.CameraBuffer, sizeof(CameraData));
    startBatch();
}


void Asura::Renderer2D::endScene() {
    ASURA_PROFILE_FUNCTION();
    flush();
}

void Asura::Renderer2D::flush() {
    if(s_data.QuadIndexCount) {
        auto dataSize = (uint32_t)((uint8_t*)s_data.QuadVertexBufferPtr - (uint8_t*)s_data.QuadVertexBufferBase);
        s_data.QuadVertexBuffer->setData(s_data.QuadVertexBufferBase, dataSize);
        // Bind textures
        for(uint32_t i = 0; i < s_data.TextureSlotIndex; i++)
            s_data.TextureSlots[i]->bind(i);
        s_data.QuadShader->bind();
        RenderCommand::drawIndexed(s_data.QuadVertexArray, s_data.QuadIndexCount);
        s_data.Stats.DrawCalls++;
    }
    
    if(s_data.CircleIndexCount) {
        auto dataSize = (uint32_t)((uint8_t*)s_data.CircleVertexBufferPtr - (uint8_t*)s_data.CircleVertexBufferBase);
        s_data.CircleVertexBuffer->setData(s_data.CircleVertexBufferBase, dataSize);
        s_data.CircleShader->bind();
        RenderCommand::drawIndexed(s_data.CircleVertexArray, s_data.CircleIndexCount);
        s_data.Stats.DrawCalls++;
    }

    if(s_data.LineVertexCount) {
        auto dataSize = (uint32_t)((uint8_t*)s_data.LineVertexBufferPtr - (uint8_t*)s_data.LineVertexBufferBase);
        s_data.LineVertexBuffer->setData(s_data.LineVertexBufferBase, dataSize);
        s_data.LineShader->bind();
        RenderCommand::setLineWidth(s_data.LineWidth);
        RenderCommand::drawLines(s_data.LineVertexArray, s_data.LineVertexCount);
        s_data.Stats.DrawCalls++;
    }
}

void Asura::Renderer2D::startBatch() {
    s_data.QuadIndexCount = 0;
    s_data.QuadVertexBufferPtr = s_data.QuadVertexBufferBase;

    s_data.CircleIndexCount = 0;
    s_data.CircleVertexBufferPtr = s_data.CircleVertexBufferBase;

    s_data.LineVertexCount = 0;
    s_data.LineVertexBufferPtr = s_data.LineVertexBufferBase;

    s_data.TextureSlotIndex = 1;
}

void Asura::Renderer2D::nextBatch() {
    flush();
    startBatch();
}

void Asura::Renderer2D::drawQuad(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color) {
    drawQuad({position.x, position.y, 0.f}, size, color);
}

void Asura::Renderer2D::drawQuad(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4 &color) {
    ASURA_PROFILE_FUNCTION();

    glm::mat4 transform = glm::translate(glm::mat4(1.f), position) * glm::scale(glm::mat4(1.f), {size.x, size.y, 1.f});

    drawQuad(transform, color);
}

void Asura::Renderer2D::drawQuad(const glm::vec2 &position, const glm::vec2 &size, Asura::Ref<Asura::Texture2D>& texture,
                                 float tilingFactor, const glm::vec4& tintColor) {
    drawQuad({position.x, position.y, 0.f}, size, texture, tilingFactor, tintColor);
}

void Asura::Renderer2D::drawQuad(const glm::vec3 &position, const glm::vec2 &size, Asura::Ref<Texture2D>& texture,
                                 float tilingFactor, const glm::vec4& tintColor) {
    glm::mat4 transform = glm::translate(glm::mat4(1.0f),position) *
                          glm::scale(glm::mat4(1.0f),{ size.x, size.y, 1.0f });

    drawQuad(transform, texture, tilingFactor, tintColor);
}

void Asura::Renderer2D::drawRotateQuad(const glm::vec2 &position, const glm::vec2 &size, float rotation, const glm::vec4 &color) {
    drawRotateQuad({position.x, position.y, 0.f}, size, rotation, color);
}

void Asura::Renderer2D::drawRotateQuad(const glm::vec3 &position, const glm::vec2 &size, float rotation, const glm::vec4 &color) {
    ASURA_PROFILE_FUNCTION();

    if(s_data.QuadIndexCount >= Renderer2DData::MaxIndices)
        nextBatch();

    glm::mat4 transform = glm::translate(glm::mat4(1.f), position) *
                          glm::scale(glm::mat4(1.f), {size.x, size.y, 1.f}) *
                          glm::rotate(glm::mat4(1.f), rotation, {0.f, 0.f, 1.f});

    drawQuad(transform, color);
}


void Asura::Renderer2D::drawRotateQuad(const glm::vec2 &position, const glm::vec2 &size, float rotation,
                                       Asura::Ref<Texture2D> &texture, float tilingFactor, const glm::vec4 &tintColor) {
    drawRotateQuad({position.x, position.y, 0.f}, size, rotation, texture, tilingFactor, tintColor);
}

void Asura::Renderer2D::drawRotateQuad(const glm::vec3 &position, const glm::vec2 &size, float rotation,
                                       Asura::Ref<Texture2D> &texture, float tilingFactor, const glm::vec4 &tintColor) {
    ASURA_PROFILE_FUNCTION();

    glm::mat4 transform = glm::translate(glm::mat4(1.f), position) *
            glm::scale(glm::mat4(1.f), {size.x, size.y, 1.f}) *
            glm::rotate(glm::mat4(1.f), rotation, {0.f, 0.f, 1.f});

    drawQuad(transform, texture, tilingFactor, tintColor);
}

void Asura::Renderer2D::drawQuad(const glm::mat4 &transform, const glm::vec4 &color, int EntityID) {
    if(s_data.QuadIndexCount >= Renderer2DData::MaxIndices)
        nextBatch();

    const float texIndex = 0.f;
    const float tilingFactor = 1.f;
    constexpr size_t quadVertexCount = 4;
    constexpr glm::vec2 texCoords[4] = {
            {0.f, 0.f}, {1.f, 0.f}, {1.f, 1.f}, {0.f, 1.f}
    };

    for(int i = 0; i < quadVertexCount; i++) {
        s_data.QuadVertexBufferPtr->Position = transform * s_data.QuadVertexPositions[i];
        s_data.QuadVertexBufferPtr->Color = color;
        s_data.QuadVertexBufferPtr->TexCoord = texCoords[i];
        s_data.QuadVertexBufferPtr->TexIndex = texIndex;
        s_data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
        s_data.QuadVertexBufferPtr->EntityID = EntityID;
        s_data.QuadVertexBufferPtr++;
    }
    s_data.QuadIndexCount += 6;
    s_data.Stats.QuadCount++;
}

void Asura::Renderer2D::drawQuad(const glm::mat4 &transform, const Asura::Ref<Asura::Texture2D> &texture,
                                 float tilingFactor, const glm::vec4 &tintColor, int EntityID) {
    if(s_data.QuadIndexCount >= Renderer2DData::MaxIndices)
        nextBatch();

    constexpr size_t quadVertexCount = 4;
    constexpr glm::vec2 texCoords[] = {
            { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f }
    };

    float texIndex = 0.f;
    for(int i = 0; i < s_data.TextureSlotIndex; i++) {
        if(s_data.TextureSlots[i] == texture) {
            texIndex = (float)i;
            break;
        }
    }
    if(texIndex == 0.f) {
        if (s_data.TextureSlotIndex >= Renderer2DData::MaxTextureSlots)
            nextBatch();

        texIndex = (float)s_data.TextureSlotIndex;
        s_data.TextureSlots[s_data.TextureSlotIndex] = texture;
        s_data.TextureSlotIndex++;
    }

    for(int i = 0; i < quadVertexCount; i++) {
        s_data.QuadVertexBufferPtr->Position = transform * s_data.QuadVertexPositions[i];
        s_data.QuadVertexBufferPtr->Color = tintColor;
        s_data.QuadVertexBufferPtr->TexCoord = texCoords[i];
        s_data.QuadVertexBufferPtr->TexIndex = texIndex;
        s_data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
        s_data.QuadVertexBufferPtr->EntityID = EntityID;
        s_data.QuadVertexBufferPtr++;
    }

    s_data.QuadIndexCount += 6;
    s_data.Stats.QuadCount++;
}

void Asura::Renderer2D::drawSprite(const glm::mat4 &transform, Asura::SpriteRendererComponent &src, int entityID) {
    if(src.Texture) {
        drawQuad(transform, src.Texture, src.TilingFactor, src.Color, entityID);
    }
    else {
        drawQuad(transform, src.Color, entityID);
    }
}

void Asura::Renderer2D::drawCircle(const glm::mat4 &transform, const glm::vec4 &color, float thickness, float fade,
                                   int entityID) {
    ASURA_PROFILE_FUNCTION();
     if(s_data.CircleIndexCount >= Renderer2DData::MaxIndices)
     	nextBatch();

    for(size_t i = 0; i < 4; i++) {
        s_data.CircleVertexBufferPtr->WorldPosition = transform * s_data.QuadVertexPositions[i];
        s_data.CircleVertexBufferPtr->LocalPosition = s_data.QuadVertexPositions[i] * 2.0f;
        s_data.CircleVertexBufferPtr->Color = color;
        s_data.CircleVertexBufferPtr->Thickness = thickness;
        s_data.CircleVertexBufferPtr->Fade = fade;
        s_data.CircleVertexBufferPtr->EntityID = entityID;
        s_data.CircleVertexBufferPtr++;
    }
    s_data.CircleIndexCount += 6;
    s_data.Stats.QuadCount++;
}

void Asura::Renderer2D::drawLine(const glm::vec3& p0, const glm::vec3& p1, const glm::vec4& color, int entityID) {
    if(s_data.LineVertexCount >= Renderer2DData::MaxIndices)
        nextBatch();

    s_data.LineVertexBufferPtr->Position = p0;
    s_data.LineVertexBufferPtr->Color = color;
    s_data.LineVertexBufferPtr->EntityID = entityID;
    s_data.LineVertexBufferPtr++;

    s_data.LineVertexBufferPtr->Position = p1;
    s_data.LineVertexBufferPtr->Color = color;
    s_data.LineVertexBufferPtr->EntityID = entityID;
    s_data.LineVertexBufferPtr++;

    s_data.LineVertexCount += 2;
}

void Asura::Renderer2D::drawRect(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4 &color,
                                 int entityID) {
    glm::vec3 p0 = glm::vec3(position.x - size.x * 0.5f, position.y - size.y * 0.5f, position.z);
    glm::vec3 p1 = glm::vec3(position.x + size.x * 0.5f, position.y - size.y * 0.5f, position.z);
    glm::vec3 p2 = glm::vec3(position.x + size.x * 0.5f, position.y + size.y * 0.5f, position.z);
    glm::vec3 p3 = glm::vec3(position.x - size.x * 0.5f, position.y + size.y * 0.5f, position.z);

    drawLine(p0, p1, color, entityID);
    drawLine(p1, p2, color, entityID);
    drawLine(p2, p3, color, entityID);
    drawLine(p3, p0, color, entityID);
}

void Asura::Renderer2D::drawRect(const glm::mat4 &transform, const glm::vec4 &color, int entityID) {
    glm::vec3 lineVertices[4];
    for (size_t i = 0; i < 4; i++)
        lineVertices[i] = transform * s_data.QuadVertexPositions[i];

    drawLine(lineVertices[0], lineVertices[1], color, entityID);
    drawLine(lineVertices[1], lineVertices[2], color, entityID);
    drawLine(lineVertices[2], lineVertices[3], color, entityID);
    drawLine(lineVertices[3], lineVertices[0], color, entityID);
}
