
#include "Mesh.h"
#include <memory>
#include "Forge/Core/Log/Log.h"
#include "Forge/Renderer/Shader.h"
#include "ShaderManager.h"


namespace Forge {
std::shared_ptr<Material> MeshTarget::m_DefaultMaterial = nullptr;

MeshTarget::MeshTarget()
{
    m_Layout = {{BufferDataType::Float3, "a_Position"},
                {BufferDataType::Float3, "a_Normal"},
                {BufferDataType::Float2, "a_TexCoords"}};
}

const BufferLayout& MeshTarget::GetLayout() const
{
    return m_Layout;
}


std::shared_ptr<Material> MeshTarget::GetDefaultMaterial()
{
    if (m_DefaultMaterial == nullptr)
    {
        auto& shaderManager = ShaderManager::GetInstance();
        Handle shaderHandle = shaderManager.LoadShader(ShaderLayout {
            {ShaderType::VERTEX,
             R"(
            #version 450 core
            layout(location = 0) in vec3 a_Position;
            layout(location = 1) in vec3 a_Norm;
            layout(location = 2) in vec2 a_TexCoord;

            layout(std140, binding = 0) uniform ViewProjection {
            mat4 u_ViewProjection;
            };

            out vec2 v_TexCoord;
            out vec3 v_Norm;
            out vec3 v_Position;

            void main()
            {
            gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
            v_TexCoord = a_TexCoord;
            v_Norm = a_Norm;
            v_Position = a_Position;
            }
        )",
             ShaderSource::STRING},


            {ShaderType::FRAGMENT,
             R"(


            #version 450 core

            layout(location = 0) out vec4 o_Color;

            in vec2 v_TexCoord;
            in vec3 v_Norm;
            in vec3 v_Position;

            void main()
            {
            o_Color = vec4(1.0, 0.0, 1.0, 1.0);
            }

        )",
             ShaderSource::STRING}});

        if (shaderHandle.GetValue() == 0)
        {
            LOG_CRITICAL("Failed to load shader.");
        }
        LOG_TRACE("Compute default Shader MeshTarget")
        m_DefaultMaterial = std::make_shared<Material>(shaderHandle);
    }

    return m_DefaultMaterial;
}

}  // namespace Forge
