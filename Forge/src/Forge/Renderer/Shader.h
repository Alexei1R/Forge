//
// Created by toor on 2024-09-11.
//

#ifndef SHADER_H
#define SHADER_H

#include "glm/glm.hpp"
#include <glad/glad.h>
#include <string>
#include <utility>
#include <vector>
#include <unordered_map>

namespace Forge {

enum class ShaderType
{
    VERTEX,
    FRAGMENT,
    GEOMETRY,
    TESSELLATION_CONTROL,
    TESSELLATION_EVALUATION,
    COMPUTE
};

class Shader
{
public:
    Shader();
    ~Shader();

    bool AddShader(const std::string& path, ShaderType shaderType);

    bool Build();
    void Reload();
    void Bind() const;
    void UnBind() const;

    void SetUniform(const std::string& name, int value);
    void SetUniform(const std::string& name, float value);
    void SetUniform(const std::string& name, const glm::vec2& value);
    void SetUniform(const std::string& name, const glm::vec3& value);
    void SetUniform(const std::string& name, const glm::vec4& value);
    void SetUniform(const std::string& name, const glm::mat2& value);
    void SetUniform(const std::string& name, const glm::mat3& value);
    void SetUniform(const std::string& name, const glm::mat4& value);

private:
    std::string ReadShader(const std::string& filePath);
    unsigned int CompileShader(const std::string& source, GLenum shaderType);
    unsigned int LinkShaders(const std::vector<unsigned int>& shaderIDs);
    int GetUniformLocation(const std::string& name);
    void BuildShader();

private:
    std::vector<std::pair<std::string, ShaderType>> m_Shaders;

    unsigned int m_ProgramID;
    unsigned int m_PreviousProgramID;
    std::unordered_map<std::string, int> m_UniformLocationCache;
};

}  // namespace Forge

#endif  // SHADER_H
