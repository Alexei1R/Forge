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
    const DrawListData GetDrawList() const override;
    void OnEvent(const Forge::Event& event) override;

    void HandleResize(const vec2i& delta, Edge selectedEdge);


    void Update();

public:
    static std::shared_ptr<Window> Create(const std::string& label);


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
};


}  // namespace BfUI


#endif
