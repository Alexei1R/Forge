//
// Created by toor on 2024-12-02.
//

#include "Window.h"
#include "Forge/BFUI/Theme.h"
#include "Forge/BFUI/Types.h"
#include "Forge/Events/ImplEvent.h"
#include "Forge/Renderer/RenderCommand.h"
#include "WidgetStack.h"

namespace BfUI {

std::shared_ptr<Window> Window::Create(const std::string& label)
{
    auto window = std::shared_ptr<Window>(new Window(label));
    WidgetStack::AddWidget(window);
    return window;
}


Window::Window(const std::string& label) : m_WindowName(label)
{
    m_WindowPosition = {50, 50};
    m_WindowSize = {200, 250};

    // NOTE: Set DefaultWindowColor
    m_ColorBackground = Theme::GetColor(WidgetType::Window, WidgetState::Default);
    Update();
}


void Window::Update()
{
    auto& panelData = DrawList::DrawPanel(m_WindowPosition, m_WindowSize, m_ColorBackground);
    m_DrawListData = panelData;
}

const DrawListData Window::GetDrawList() const
{
    return m_DrawListData;
}


void Window::OnEvent(const Forge::Event& event)
{
    auto [x, y] = Forge::Mouse::GetMousePosition();
    auto [appX, appY] = Forge::ApplicationStats::GetApplicationSize();

    glm::vec2 mousePos(x, appY - y);  // Flip Y coordinate

    if (event.GetType() == Forge::EventType::Key)
    {
        const auto& keyEvent = static_cast<const Forge::KeyEvent&>(event);

        if (keyEvent.GetAction() == Forge::Action::MousePress)
        {
            selectedEdge = IsNearEdge(m_WindowPosition, m_WindowSize);
            if (selectedEdge != Edge::None)
            {
                m_InitialWindowPosition = m_WindowPosition;
                m_InitialWindowSize = m_WindowSize;
                m_InitialMousePos = mousePos;
                m_IsAnyEdgeSelected = true;
            }
        }
        else if (keyEvent.GetAction() == Forge::Action::MouseRelease)
        {
            if (m_IsAnyEdgeSelected)
            {
                Forge::RenderCommand::SetCursorType(Forge::CursorType::Arrow);
                m_IsAnyEdgeSelected = false;
            }
        }
    }
    else if (event.GetType() == Forge::EventType::Mouse)
    {
        const auto& mouseEvent = static_cast<const Forge::MouseEvent&>(event);

        // Handle mouse move for hover
        if (mouseEvent.GetAction() == Forge::Action::MouseMove)
        {
            if (m_IsAnyEdgeSelected)
            {
                glm::vec2 delta;
                delta.x = mousePos.x - m_InitialMousePos.x;
                delta.y = mousePos.y - m_InitialMousePos.y;
                HandleResize(delta, selectedEdge);
                Update();
            }
        }
    }
}


const bool Window::IsInBounds(const glm::vec2& point) const
{
    return point.x >= m_WindowPosition.x && point.x <= m_WindowPosition.x + m_WindowSize.x &&
           point.y >= m_WindowPosition.y && point.y <= m_WindowPosition.y + m_WindowSize.y;
}

const Edge Window::IsNearEdge(const vec2i& position, const vec2i& size, const int threshold) const
{
    vec2i bottomLeft = position;
    vec2i topRight = bottomLeft + size;

    // Get mouse position
    auto [mouseX, mouseY] = Forge::Mouse::GetMousePosition();
    auto [appWidth, appHeight] = Forge::ApplicationStats::GetApplicationSize();
    glm::vec2 mousePos(mouseX, appHeight - mouseY);

    float left = bottomLeft.x;
    float right = topRight.x;
    float bottom = bottomLeft.y;
    float top = topRight.y;

    bool withinHorizontalBounds =
        (mousePos.x >= left - threshold) && (mousePos.x <= right + threshold);
    bool withinVerticalBounds =
        (mousePos.y >= bottom - threshold) && (mousePos.y <= top + threshold);

    bool nearLeft =
        (std::abs(mousePos.x - left) <= threshold) && (mousePos.y >= bottom) && (mousePos.y <= top);
    bool nearRight = (std::abs(mousePos.x - right) <= threshold) && (mousePos.y >= bottom) &&
                     (mousePos.y <= top);
    bool nearBottom = (std::abs(mousePos.y - bottom) <= threshold) && (mousePos.x >= left) &&
                      (mousePos.x <= right);
    bool nearTop =
        (std::abs(mousePos.y - top) <= threshold) && (mousePos.x >= left) && (mousePos.x <= right);

    // Check corners first
    if (nearLeft && nearBottom)
        return Edge::CornerBottomLeft;
    if (nearLeft && nearTop)
        return Edge::CornerTopLeft;
    if (nearRight && nearBottom)
        return Edge::CornerBottomRight;
    if (nearRight && nearTop)
        return Edge::CornerTopRight;

    // Check edges
    if (nearLeft)
        return Edge::Left;
    if (nearRight)
        return Edge::Right;
    if (nearBottom)
        return Edge::Bottom;
    if (nearTop)
        return Edge::Top;

    return Edge::None;
}


void Window::HandleResize(const vec2i& delta, Edge selectedEdge)
{
    const vec2i minSize(100.0f, 100.0f);

    switch (selectedEdge)
    {
        case Edge::CornerTopRight:
            m_WindowSize.x = std::max(m_InitialWindowSize.x + delta.x, minSize.x);
            m_WindowSize.y = std::max(m_InitialWindowSize.y + delta.y, minSize.y);
            Forge::RenderCommand::SetCursorType(Forge::CursorType::ResizeAll);
            break;

        case Edge::CornerBottomLeft:
            m_WindowPosition.x = m_InitialWindowPosition.x + delta.x;
            m_WindowPosition.y = m_InitialWindowPosition.y + delta.y;
            m_WindowSize.x = std::max(m_InitialWindowSize.x - delta.x, minSize.x);
            m_WindowSize.y = std::max(m_InitialWindowSize.y - delta.y, minSize.y);
            Forge::RenderCommand::SetCursorType(Forge::CursorType::ResizeAll);
            break;

        case Edge::CornerBottomRight:
            m_WindowPosition.y = m_InitialWindowPosition.y + delta.y;
            m_WindowSize.y = std::max(m_InitialWindowSize.y - delta.y, minSize.y);
            m_WindowSize.x = std::max(m_InitialWindowSize.x + delta.x, minSize.x);
            Forge::RenderCommand::SetCursorType(Forge::CursorType::ResizeAll);
            break;

        case Edge::CornerTopLeft:
            m_WindowSize.y = std::max(m_InitialWindowSize.y + delta.y, minSize.y);
            m_WindowPosition.x = m_InitialWindowPosition.x + delta.x;
            m_WindowSize.x = std::max(m_InitialWindowSize.x - delta.x, minSize.x);
            Forge::RenderCommand::SetCursorType(Forge::CursorType::ResizeAll);
            break;

        case Edge::Right:
            m_WindowSize.x = std::max(m_InitialWindowSize.x + delta.x, minSize.x);
            Forge::RenderCommand::SetCursorType(Forge::CursorType::HResize);
            break;

        case Edge::Top:
            m_WindowSize.y = std::max(m_InitialWindowSize.y + delta.y, minSize.y);
            Forge::RenderCommand::SetCursorType(Forge::CursorType::VResize);
            break;

        case Edge::Left:
            m_WindowPosition.x = m_InitialWindowPosition.x + delta.x;
            m_WindowSize.x = std::max(m_InitialWindowSize.x - delta.x, minSize.x);
            Forge::RenderCommand::SetCursorType(Forge::CursorType::HResize);
            break;

        case Edge::Bottom:
            m_WindowPosition.y = m_InitialWindowPosition.y + delta.y;
            m_WindowSize.y = std::max(m_InitialWindowSize.y - delta.y, minSize.y);
            Forge::RenderCommand::SetCursorType(Forge::CursorType::VResize);
            break;

        default:
            // Optionally handle other cases or do nothing
            break;
    }

    // Optionally, enforce maximum window size based on application size
    const auto [appWidth, appHeight] = Forge::ApplicationStats::GetApplicationSize();
    m_WindowSize.x = std::min(m_WindowSize.x, static_cast<int>(appWidth));
    m_WindowSize.y = std::min(m_WindowSize.y, static_cast<int>(appHeight));
}
}  // namespace BfUI
