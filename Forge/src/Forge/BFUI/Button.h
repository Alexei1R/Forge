
#ifndef BUTTON_H
#define BUTTON_H

#include <string>

#include "Forge/BFUI/Widget.h"
namespace bf {

class Button : public Widget, public std::enable_shared_from_this<Button> {
protected:
    Button(const vec2i& size, const std::string& label);
    const bool IsInBounds(const glm::vec2& point) const;
    const DrawListData GetDrawList() override;

public:
    static std::shared_ptr<Button> Create(const vec2i& size, const std::string& label);
    vec2i GetPosition() const override;
    vec2i GetSize() const override;
    vec4i GetPadding() const override;

    std::shared_ptr<Widget> SetPosition(const vec2i& position) override;
    std::shared_ptr<Widget> SetSize(const vec2i& size) override;
    std::shared_ptr<Widget> SetPadding(const vec4i& padding) override;

    std::shared_ptr<Widget> SubscribeEvents(EventCallback callback) override;
    std::shared_ptr<Widget> SetText(const std::string& label) override;

    // WARN: Is Not intended to be used by user
    void SetParent(std::shared_ptr<Widget> parentWidget) override;
    void AddChild(std::shared_ptr<Widget> child) override;
    void OnEvent(const Forge::Event& event) override;
    void Update();

private:
    std::string m_Name;
    vec2i m_Size;
    vec2i m_Position;
    EventCallback m_EventCallback;

    DrawListData m_DrawListData;

    bool m_IsPressed = false;
    bool m_IsHovered = false;

    vec4f m_ColorBackground;
    vec4f m_ColorBackgroundDefault;
    vec4f m_ColorBackgroundHover;
    vec4f m_ColorBackgroundPressed;

    vec2i m_InitialMousePos;
    vec2i m_InitialPosition;
    vec2i m_InitialSize;
    vec2i m_MinSize;
    vec4i m_Padding;

    // WARN: Layout Usage Code
    std::vector<std::shared_ptr<Widget>> m_Children;
    std::shared_ptr<Widget> m_ParentWidget;
};

} // namespace bf

#endif
