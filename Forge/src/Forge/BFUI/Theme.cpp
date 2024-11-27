
//
// Created by toor on 2024-11-26.
//


#include "Theme.h"

namespace BfUI {

std::unordered_map<WidgetType, std::unordered_map<WidgetState, glm::vec4>> Theme::m_Colors;
std::unordered_map<WidgetType, float> Theme::m_Rounding;

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

void Theme::SetDarkTheme()
{
    // Panel Colors
    SetColor(WidgetType::Panel, WidgetState::Default, glm::vec4(0.180f, 0.180f, 0.180f, 1.000f));
    SetColor(WidgetType::Panel, WidgetState::Disabled, glm::vec4(0.180f, 0.180f, 0.180f, 0.500f));
    SetRounding(WidgetType::Panel, 4.0f);

    // Button Colors
    SetColor(WidgetType::Button, WidgetState::Default, glm::vec4(0.160f, 0.160f, 0.160f, 1.000f));
    SetColor(WidgetType::Button, WidgetState::Hover, glm::vec4(0.200f, 0.200f, 0.200f, 1.000f));
    SetColor(WidgetType::Button, WidgetState::Pressed, glm::vec4(0.280f, 0.280f, 0.280f, 1.000f));
    SetColor(WidgetType::Button, WidgetState::Disabled, glm::vec4(0.500f, 0.500f, 0.500f, 0.500f));
    SetRounding(WidgetType::Button, 14.0f);

    // Window Colors
    SetColor(WidgetType::Window, WidgetState::Default, glm::vec4(0.195f, 0.195f, 0.195f, 1.000f));
    SetRounding(WidgetType::Window, 4.0f);

    // Popup Colors
    SetColor(WidgetType::Popup, WidgetState::Default, glm::vec4(0.313f, 0.313f, 0.313f, 1.000f));
    SetRounding(WidgetType::Popup, 2.0f);

    // Scrollbar Colors
    SetColor(WidgetType::Scrollbar,
             WidgetState::Default,
             glm::vec4(0.160f, 0.160f, 0.160f, 1.000f));
    SetColor(WidgetType::Scrollbar, WidgetState::Hover, glm::vec4(0.300f, 0.300f, 0.300f, 1.000f));
    SetColor(WidgetType::Scrollbar,
             WidgetState::Pressed,
             glm::vec4(1.000f, 0.391f, 0.000f, 1.000f));
    SetRounding(WidgetType::Scrollbar, 12.0f);

    // Slider Colors
    SetColor(WidgetType::Slider, WidgetState::Default, glm::vec4(0.391f, 0.391f, 0.391f, 1.000f));
    SetColor(WidgetType::Slider, WidgetState::Hover, glm::vec4(0.469f, 0.469f, 0.469f, 1.000f));
    SetColor(WidgetType::Slider, WidgetState::Pressed, glm::vec4(1.000f, 0.391f, 0.000f, 1.000f));
    SetRounding(WidgetType::Slider, 2.0f);

    // Text Colors
    SetColor(WidgetType::Text, WidgetState::Default, glm::vec4(1.000f, 1.000f, 1.000f, 1.000f));
    SetColor(WidgetType::Text, WidgetState::Disabled, glm::vec4(0.500f, 0.500f, 0.500f, 1.000f));

    // Checkmark Colors
    SetColor(WidgetType::Checkmark,
             WidgetState::Default,
             glm::vec4(1.000f, 1.000f, 1.000f, 1.000f));
    SetRounding(WidgetType::Checkmark, 0.0f);  // No rounding needed for checkmark
}

}  // namespace BfUI
