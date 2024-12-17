

#include "Widget.h"

#include "Forge/BFUI/Theme.h"
#include "Forge/Renderer/Font.h"
#include "Forge/Renderer/ShaderManager.h"

namespace bf {

Widget::Widget()
    : m_Padding(vec4i(8)) {
    Theme::SetDarkTheme();
    auto& shaderManager = Forge::ShaderManager::GetInstance();
    Handle shaderHandle =
        shaderManager.LoadShader(Forge::ShaderLayout{{Forge::ShaderType::VERTEX, "Assets/Shaders/UI/Default.vert"},
                                                     {Forge::ShaderType::FRAGMENT, "Assets/Shaders/UI/Default.frag"}});

    if (shaderHandle.GetValue() == 0) {
        LOG_CRITICAL("Failed to load shader.");
    }
    m_DefaultMaterial = std::make_shared<Forge::Material>(shaderHandle);

    m_DefaultMaterial->SetParameter("BorderSize", (int)Theme::GetSize(WidgetType::Border));
    m_DefaultMaterial->SetParameter("BorderColor", Theme::GetColor(WidgetType::Border, WidgetState::Default));

    float feather = 1.0f;
    m_DefaultMaterial->SetParameter("Feather", feather);
    m_DefaultMaterial->SetParameter("Radius", (int)Theme::GetRounding(WidgetType::Window));

    m_WidgetBufferLayout = {{Forge::BufferDataType::Float2, "a_Position"}, {Forge::BufferDataType::Float4, "a_Color"},
                            {Forge::BufferDataType::Float2, "a_TexCoord"}, {Forge::BufferDataType::Float, "a_TexIndex"},
                            {Forge::BufferDataType::Float2, "a_QuadSize"}, {Forge::BufferDataType::Float, "a_Type"}};

    m_DefaultMaterial->SetTexture(0, Forge::Font::GetDefault()->GetAtlasTextureHandle());
}

const std::shared_ptr<Forge::Material>& Widget::GetMaterial() const {
    return m_DefaultMaterial;
};

const Forge::BufferLayout& Widget::GetLayout() const {
    { return m_WidgetBufferLayout; };
}

} // namespace bf
