

#include "Theme.h"

namespace bf {

std::unordered_map<WidgetType, std::unordered_map<WidgetState, glm::vec4>> Theme::m_Colors;
std::unordered_map<WidgetType, float> Theme::m_Rounding;
std::unordered_map<WidgetType, float> Theme::m_Size;

void Theme::SetColor(WidgetType widget, WidgetState state, const glm::vec4& color)
{
    m_Colors[widget][state] = color;
}

glm::vec4 Theme::GetColor(WidgetType widget, WidgetState state)
{
    if (m_Colors.find(widget) != m_Colors.end() &&
        m_Colors.at(widget).find(state) != m_Colors.at(widget).end())
    {
        return m_Colors.at(widget).at(state);
    }
    return glm::vec4(1.0f);  // White color as fallback
}

void Theme::SetRounding(WidgetType widget, float rounding)
{
    m_Rounding[widget] = rounding;
}

float Theme::GetRounding(WidgetType widget)
{
    if (m_Rounding.find(widget) != m_Rounding.end())
    {
        return m_Rounding.at(widget);
    }
    return 0.0f;  // No rounding as fallback
}


void Theme::SetSize(WidgetType widget, float size)
{
    m_Size[widget] = size;
}

float Theme::GetSize(WidgetType widget)
{
    if (m_Size.find(widget) != m_Size.end())
    {
        return m_Size.at(widget);
    }
    return 0.0f;  // No rounding as fallback
}

void Theme::SetDarkTheme()
{
    // Border
    SetColor(WidgetType::Border, WidgetState::Default, glm::vec4(0.7, 0.7, 0.7, 1.0));
    SetSize(WidgetType::Border, 2.5f);

    // Window Colors
    SetColor(WidgetType::Window, WidgetState::Default, glm::vec4(0.195f, 0.195f, 0.195f, 0.5f));
    SetRounding(WidgetType::Window, 8.0f);

    // Button Colors
    SetColor(WidgetType::Button, WidgetState::Default, glm::vec4(0.150f, 0.350f, 0.850f, 1.000f));
    SetColor(WidgetType::Button, WidgetState::Hover, glm::vec4(0.200f, 0.450f, 1.000f, 1.000f));
    SetColor(WidgetType::Button, WidgetState::Pressed, glm::vec4(0.100f, 0.250f, 0.700f, 1.000f));
    SetColor(WidgetType::Button, WidgetState::Disabled, glm::vec4(0.150f, 0.350f, 0.850f, 0.300f));
    SetRounding(WidgetType::Button, 20.0f);

    // Text Colors
    SetColor(WidgetType::Text, WidgetState::Default, glm::vec4(1.000f, 1.000f, 1.000f, 1.000f));
    SetColor(WidgetType::Text, WidgetState::Disabled, glm::vec4(0.500f, 0.500f, 0.500f, 1.000f));
}

}  // namespace bf
