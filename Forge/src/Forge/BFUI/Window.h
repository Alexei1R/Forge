
#ifndef BFWINDOW_H
#define BFWINDOW_H


#include "Forge/BFUI/Widget.h"
#include <string>
namespace bf {

class Window : public Widget, public std::enable_shared_from_this<Window>
{
protected:
    Window(const std::string& label);
    const bool IsInBounds(const glm::vec2& point) const;
    const Edge IsNearEdge(const vec2i& position, const vec2i& size, const int threshold = 15) const;
    const DrawListData GetDrawList() override;
    void HandleResize(const vec2i& delta, Edge selectedEdge);
    void HandleDragg(const vec2i& delta);

public:
    static std::shared_ptr<Window> Create(const std::string& label);
    vec2i GetPosition() const override;
    vec2i GetSize() const override;
    vec4i GetPadding() const override;
    std::shared_ptr<Widget> SetPosition(const vec2i& position) override;
    std::shared_ptr<Widget> SetSize(const vec2i& size) override;
    std::shared_ptr<Widget> SetPadding(const vec4i& padding) override;

    // WARN: Is Not intended to be used by user
    void SetParent(std::shared_ptr<Widget> parentWidget) override;
    void AddChild(std::shared_ptr<Widget> child) override;
    void OnEvent(const Forge::Event& event) override;
    void Update();

private:
    std::string m_Name;
    vec2i m_Size;
    vec2i m_Position;

    DrawListData m_DrawListData;
    vec4f m_ColorBackground;

    vec2i m_InitialMousePos;
    vec2i m_InitialPosition;
    vec2i m_InitialSize;
    vec2i m_MinSize;
    vec4i m_Padding;
    Edge selectedEdge = Edge::None;
    bool m_IsAnyEdgeSelected = false;
    bool m_IsWindowDragged = false;


    // WARN: Layout Usage Code
    std::vector<std::shared_ptr<Widget>> m_Children;
    std::shared_ptr<Widget> m_ParentWidget;
};


}  // namespace bf


#endif
