
#ifndef THEME_H
#define THEME_H

#include <glm/vec4.hpp>
#include <string>
#include <unordered_map>

namespace bf {

enum class WidgetType {
    Panel,
    Button,
    Window,
    Popup,
    Scrollbar,
    Slider,
    Text,
    Checkmark,
    Border,
};

enum class WidgetState {
    Default,
    Hover,
    Pressed,
    Disabled,
};

class Theme {
public:
    static void SetColor(WidgetType widget, WidgetState state, const glm::vec4& color);
    static glm::vec4 GetColor(WidgetType widget, WidgetState state);

    static void SetRounding(WidgetType widget, float rounding);
    static float GetRounding(WidgetType widget);

    static void SetSize(WidgetType widget, float size);
    static float GetSize(WidgetType widget);

    static void SetDarkTheme();

private:
    static std::unordered_map<WidgetType, std::unordered_map<WidgetState, glm::vec4>> m_Colors;
    static std::unordered_map<WidgetType, float> m_Rounding;
    static std::unordered_map<WidgetType, float> m_Size;
};

} // namespace bf

#endif
