

#include "Shader.h"
#include "Forge/Core/Log/Log.h"

#include <iostream>
#include <fstream>
#include <sstream>


namespace Forge {

ShaderLayout::ShaderLayout() {}

ShaderLayout::ShaderLayout(std::initializer_list<ShaderElement> elements) : m_Elements(elements) {}

void ShaderLayout::AddElement(const ShaderElement& element)
{
    m_Elements.push_back(element);
}

std::vector<ShaderElement>::iterator ShaderLayout::begin()
{
    return m_Elements.begin();
}

std::vector<ShaderElement>::iterator ShaderLayout::end()
{
    return m_Elements.end();
}

std::vector<ShaderElement>::const_iterator ShaderLayout::begin() const
{
    return m_Elements.begin();
}

std::vector<ShaderElement>::const_iterator ShaderLayout::end() const
{
    return m_Elements.end();
}

Shader::Shader() {}

Shader::Shader(const ShaderLayout& layout) : m_ShaderLayout(layout)
{
    BuildShader();
}

Shader::~Shader()
{
    if (m_ProgramID != 0)
    {
        glDeleteProgram(m_ProgramID);
        m_ProgramID = 0;
    }
}

bool Shader::Build()
{
    BuildShader();
    this->Bind();
    return m_ProgramID != 0;
}

void Shader::Reload()
{
    BuildShader();
    this->Bind();
}

void Shader::Bind() const
{
    glUseProgram(m_ProgramID);
}

void Shader::UnBind() const
{
    glUseProgram(0);
}

void Shader::BuildShader()
{
    if (m_ProgramID != 0)
    {
        m_PreviousProgramID = m_ProgramID;
    }

    std::vector<unsigned int> compiledShaders;

    for (const auto& shader : m_ShaderLayout)
    {
        std::string source = ReadShader(shader.path);
        if (source.empty())
        {
            LOG_CRITICAL("Shader source is empty: " + shader.path);
            continue;
        }

        GLenum glShaderType;
        switch (shader.type)
        {
            case ShaderType::VERTEX: glShaderType = GL_VERTEX_SHADER; break;
            case ShaderType::FRAGMENT: glShaderType = GL_FRAGMENT_SHADER; break;
            case ShaderType::GEOMETRY: glShaderType = GL_GEOMETRY_SHADER; break;
            case ShaderType::TESSELLATION_CONTROL: glShaderType = GL_TESS_CONTROL_SHADER; break;
            case ShaderType::TESSELLATION_EVALUATION:
                glShaderType = GL_TESS_EVALUATION_SHADER;
                break;
            case ShaderType::COMPUTE: glShaderType = GL_COMPUTE_SHADER; break;
            default: LOG_CRITICAL("Unknown shader type."); continue;
        }

        unsigned int shaderID = CompileShader(source, glShaderType);
        if (shaderID != 0)
        {
            compiledShaders.push_back(shaderID);
        }
        else
        {
            LOG_CRITICAL("Failed to compile shader: " + shader.path);
        }
    }

    m_ProgramID = LinkShaders(compiledShaders);

    if (m_ProgramID == 0)
    {
        m_ProgramID = m_PreviousProgramID;
        LOG_CRITICAL("Build failed, reverted to the previous successful build if it exists.");
    }
}

std::string Shader::ReadShader(const std::string& filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open())
    {
        LOG_CRITICAL("Could not open shader file: " + filePath);
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

unsigned int Shader::CompileShader(const std::string& source, GLenum shaderType)
{
    unsigned int shaderID = glCreateShader(shaderType);
    const char* src = source.c_str();
    glShaderSource(shaderID, 1, &src, nullptr);
    glCompileShader(shaderID);

    int success;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[1024];
        glGetShaderInfoLog(shaderID, 1024, nullptr, infoLog);
        LOG_CRITICAL("Shader compilation failed:\n" + std::string(infoLog));
        glDeleteShader(shaderID);
        return 0;
    }

    return shaderID;
}

unsigned int Shader::LinkShaders(const std::vector<unsigned int>& shaderIDs)
{
    if (shaderIDs.empty())
    {
        LOG_CRITICAL("No shaders to link. Please add shaders before building.");
        return 0;
    }

    unsigned int programID = glCreateProgram();

    for (unsigned int shaderID : shaderIDs)
    {
        glAttachShader(programID, shaderID);
    }

    glLinkProgram(programID);

    int success;
    glGetProgramiv(programID, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[1024];
        glGetProgramInfoLog(programID, 1024, nullptr, infoLog);
        LOG_CRITICAL("Shader program linking failed:\n" + std::string(infoLog));
        glDeleteProgram(programID);

        for (unsigned int shaderID : shaderIDs)
        {
            glDeleteShader(shaderID);
        }

        return 0;
    }

    for (unsigned int shaderID : shaderIDs)
    {
        glDetachShader(programID, shaderID);
        glDeleteShader(shaderID);
    }

    return programID;
}

int Shader::GetUniformLocation(const std::string& name)
{
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
    {
        return m_UniformLocationCache[name];
    }

    int location = glGetUniformLocation(m_ProgramID, name.c_str());
    /*if (location == -1)*/
    /*{*/
    /*    LOG_WARN("Uniform '" + name + "' doesn't exist or is not used.");*/
    /*}*/
    /**/
    m_UniformLocationCache[name] = location;
    return location;
}

void Shader::SetUniform(const std::string& name, int value)
{
    glUniform1i(GetUniformLocation(name), value);
}

void Shader::SetUniform(const std::string& name, float value)
{
    glUniform1f(GetUniformLocation(name), value);
}

void Shader::SetUniform(const std::string& name, const glm::vec2& value)
{
    glUniform2f(GetUniformLocation(name), value.x, value.y);
}

void Shader::SetUniform(const std::string& name, const glm::vec3& value)
{
    glUniform3f(GetUniformLocation(name), value.x, value.y, value.z);
}

void Shader::SetUniform(const std::string& name, const glm::vec4& value)
{
    glUniform4f(GetUniformLocation(name), value.x, value.y, value.z, value.w);
}

void Shader::SetUniform(const std::string& name, const glm::mat2& value)
{
    glUniformMatrix2fv(GetUniformLocation(name), 1, GL_FALSE, &value[0][0]);
}

void Shader::SetUniform(const std::string& name, const glm::mat3& value)
{
    glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, &value[0][0]);
}

void Shader::SetUniform(const std::string& name, const glm::mat4& value)
{
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &value[0][0]);
}

}  // namespace Forge
