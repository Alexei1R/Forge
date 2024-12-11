
#include "Mesh.h"
#include <memory>
#include "ShaderManager.h"


namespace Forge {
std::shared_ptr<Material> MeshTarget::m_DefaultMaterial = nullptr;

MeshTarget::MeshTarget()
{
    m_Layout = {{BufferDataType::Float3, "a_Position"},
                {BufferDataType::Float3, "a_Normal"},
                {BufferDataType::Float2, "a_TexCoords"}};


    auto& shaderManager = ShaderManager::GetInstance();
    Handle shaderHandle = shaderManager.LoadShader(
        ShaderLayout {{ShaderType::VERTEX, "Assets/Shaders/Mesh/Default.vert"},
                      {ShaderType::FRAGMENT, "Assets/Shaders/Mesh/Default.frag"}});

    if (shaderHandle.GetValue() == 0)
    {
        LOG_CRITICAL("Failed to load shader.");
    }
    m_DefaultMaterial = std::make_shared<Material>(shaderHandle);
}

const BufferLayout& MeshTarget::GetLayout() const
{
    return m_Layout;
}


std::shared_ptr<Material> MeshTarget::GetDefaultMaterial()
{
    return m_DefaultMaterial;
}

}  // namespace Forge
