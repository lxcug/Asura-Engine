// @author: lx
// @time: 2022/10/15
// @brief: 

#pragma once
#ifndef GAME_SHADER_H
#define GAME_SHADER_H

#include "Asura/Core/Core.h"
#include "glm/glm.hpp"
#include "Glad/glad.h"
#include "Asura/Core/Log.h"
#include "Renderer/RendererAPI.h"

#include <iostream>


namespace Asura {
    class ASURA_API Shader {
    public:
        virtual ~Shader() = default;

        virtual void bind() const = 0;
        virtual void unbind() const = 0;

        virtual void setInt(const std::string& name, int value) = 0;
        virtual void setFloat(const std::string& name, float value) = 0;
        virtual void setFloat3(const std::string& name, const glm::vec3& value) = 0;
        virtual void setFloat4(const std::string& name, const glm::vec4& value) = 0;
        virtual void setMat4(const std::string& name, const glm::mat4& value) = 0;
        virtual void setIntArray(const std::string& name, int* values, unsigned count) = 0;

        [[nodiscard]] virtual const std::string& getName() const = 0;

        static Ref<Shader> createShader(const std::string& filePath);
        static Ref<Shader> createShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
    };


    class ASURA_API ShaderLibrary {
    public:
        void addShader(Ref<Shader>& shader);
        void addShader(const std::string& name, Ref<Shader>& shader);
        void addShader(const std::string& name, const std::string& filePath);

        Ref<Shader> loadShader(const std::string& filePath);
        Ref<Shader> loadShader(const std::string&name, const std::string& vertexSrc, const std::string& fragmentSrc);

        Ref<Shader> getShader(const std::string& name);
        bool exists(const std::string& name) const;

    private:
        std::unordered_map<std::string, Ref<Shader>> m_shaders;
    };
}


#endif //GAME_SHADER_H
