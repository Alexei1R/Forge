//
// Created by toor on 2024-11-28.
//

#ifndef BUTTON_H
#define BUTTON_H


#include "Forge/BFUI/DrawList.h"
#include "Forge/BFUI/Widget.h"
#include "Types.h"
#include <memory>
namespace BfUI {

class Button : public Widget, public std::enable_shared_from_this<Button>
{
protected:
    using EventCallback = std::function<void(WidgetEvent, Widget&)>;
    Button(const vec2i& position, const vec2i& size, const std::string& label);

    void OnEvent(const Forge::Event& event) override;
    const DrawListData GetDrawList() const override;

public:
    static std::shared_ptr<Button>
        Create(const vec2i& position, const vec2i& size, const std::string& label);
    ~Button() = default;

    void SubscribeEvents(EventCallback callback);

    void Move(vec2i position);


    void Update();

private:
    const bool IsInBounds(const glm::vec2& point) const;

private:
    EventCallback m_EventCallback;
    DrawListData m_DrawListData;
    vec2i m_BtnPosition;
    vec2i m_BtnSize;
    std::string m_BtnLabel;

    bool m_IsPressed = false;
    bool m_IsHovered = false;


    vec4f m_ColorBackground;
    vec4f m_ColorBackgroundDefault;
    vec4f m_ColorBackgroundHover;
    vec4f m_ColorBackgroundPressed;
};

}  // namespace BfUI


#endif
