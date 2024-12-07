//
// Created by toor on 2024-12-02.
//

#ifndef BFWINDOW_H
#define BFWINDOW_H


#include "Forge/BFUI/Widget.h"
#include <string>
namespace BfUI {


class Window : public Widget, public std::enable_shared_from_this<Window>
{
protected:
    Window(const std::string& label);

protected:
    const bool IsInBounds(const glm::vec2& point) const;
    const Edge IsNearEdge(const vec2i& position, const vec2i& size, const int threshold = 10) const;
    const DrawListData GetDrawList() override;

    void HandleResize(const vec2i& delta, Edge selectedEdge);


public:
    static std::shared_ptr<Window> Create(const std::string& label);

    void AddChild(std::shared_ptr<Widget> child) override;

    void OnEvent(const Forge::Event& event) override;
    void Update();

    vec2i GetPosition() const override;
    vec2i GetSize() const override;
    void SetPosition(const vec2i& position) override;
    void SetSize(const vec2i& size) override;

    void SetParent(std::shared_ptr<Widget> parentWidget) override;

private:
    std::string m_WindowName;
    vec2i m_WindowSize;
    vec2i m_WindowPosition;

    DrawListData m_DrawListData;

    bool m_IsAnyEdgeSelected = false;


    vec4f m_ColorBackground;

    vec2i m_InitialMousePos;
    vec2i m_InitialWindowPosition;
    vec2i m_InitialWindowSize;
    Edge selectedEdge = Edge::None;

    std::shared_ptr<Widget> m_ParentWidget;
    // NOTE: Layout Usage Code
    std::vector<std::shared_ptr<Widget>> m_Children;
};


}  // namespace BfUI


#endif
