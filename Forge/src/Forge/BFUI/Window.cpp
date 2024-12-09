//
// Created by toor on 2024-12-02.
//

#include "Window.h"
#include "Forge/BFUI/Theme.h"
#include "Forge/BFUI/Types.h"
#include "Forge/Core/Log/Log.h"
#include "Forge/Events/ImplEvent.h"
#include "Forge/Renderer/RenderCommand.h"
#include "WidgetStack.h"

namespace BfUI {

std::shared_ptr<Window> Window::Create(const std::string& label)
{
    return std::shared_ptr<Window>(new Window(label));
}


Window::Window(const std::string& label) : m_WindowName(label)
{
    m_WindowPosition = {250, 250};
    m_WindowSize = {500, 500};

    // NOTE: Set DefaultWindowColor
    m_ColorBackground = Theme::GetColor(WidgetType::Window, WidgetState::Default);
    Update();
}


void Window::SetParent(std::shared_ptr<Widget> parentWidget)
{
    m_ParentWidget = parentWidget;
}


vec2i Window::GetPosition() const
{
    return m_WindowPosition;
}
vec2i Window::GetSize() const
{
    return m_WindowSize;
}
void Window::SetPosition(const vec2i& position)
{
    if (m_WindowPosition != position)
    {
        m_WindowPosition = position;
        Update();
    }
}
void Window::SetSize(const vec2i& size)
{
    if (m_WindowSize != size)
    {
        m_WindowSize = size;
        Update();
    }
}

void Window::AddChild(std::shared_ptr<Widget> child)
{
    child->SetParent(shared_from_this());
    m_Children.push_back(child);
    Update();
}

void Window::Update()
{
    DrawListData dataStore;
    for (auto& child : m_Children)
    {
        DrawListData data = child->GetDrawList();

        dataStore += data;
    }
    /*LOG_WARN("Size {} {}", m_WindowSize.x, m_WindowSize.y)*/
    m_DrawListData =
        dataStore + DrawList::DrawPanel(m_WindowPosition, m_WindowSize, m_ColorBackground);
}

const DrawListData Window::GetDrawList()
{
    return m_DrawListData;
}


void Window::OnEvent(const Forge::Event& event)
{
    for (auto& child : m_Children)
    {
        child->OnEvent(event);
    }
    auto [x, y] = Forge::Mouse::GetMousePosition();

    glm::vec2 mousePos(x, y);

    /*selectedEdge = IsNearEdge(m_WindowPosition, m_WindowSize);*/
    /*if (selectedEdge != Edge::None)*/
    /*{*/
    /*    LOG_INFO("Grab edge")*/
    /*    m_InitialWindowPosition = m_WindowPosition;*/
    /*    m_InitialWindowSize = m_WindowSize;*/
    /*    m_InitialMousePos = mousePos;*/
    /*    m_IsAnyEdgeSelected = true;*/
    /*}*/
    /**/
    if (event.GetType() == Forge::EventType::Key)
    {
        const auto& keyEvent = static_cast<const Forge::KeyEvent&>(event);


        if (keyEvent.GetAction() == Forge::Action::MousePress)

        {
            selectedEdge = IsNearEdge(m_WindowPosition, m_WindowSize);
            if (selectedEdge != Edge::None)
            {
                LOG_INFO("Grab edge")
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
    vec2i topLeft = position;
    vec2i bottomRight = topLeft + size;

    // Get mouse position
    auto [mouseX, mouseY] = Forge::Mouse::GetMousePosition();
    glm::vec2 mousePos(mouseX, mouseY);

    float left = topLeft.x;
    float right = bottomRight.x;
    float top = topLeft.y;
    float bottom = bottomRight.y;

    LOG_WARN("Left: {}, Right: {}, Bottom: {}, Top: {}", left, right, bottom, top);
    LOG_TRACE("Mouse {} ", std::abs(mousePos.x - left));


    bool nearLeft =
        (std::abs(mousePos.x - left) <= threshold) && (mousePos.y <= bottom) && (mousePos.y >= top);
    bool nearRight = (std::abs(mousePos.x - right) <= threshold) && (mousePos.y <= bottom) &&
                     (mousePos.y >= top);
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
        case Edge::CornerTopLeft:
            m_WindowPosition.x = m_InitialWindowPosition.x + delta.x;
            m_WindowPosition.y = m_InitialWindowPosition.y + delta.y;
            m_WindowSize.x = std::max(m_InitialWindowSize.x - delta.x, minSize.x);
            m_WindowSize.y = std::max(m_InitialWindowSize.y - delta.y, minSize.y);
            Forge::RenderCommand::SetCursorType(Forge::CursorType::ResizeAll);
            break;

        case Edge::CornerBottomRight:
            m_WindowSize.x = std::max(m_InitialWindowSize.x + delta.x, minSize.x);
            m_WindowSize.y = std::max(m_InitialWindowSize.y + delta.y, minSize.y);
            Forge::RenderCommand::SetCursorType(Forge::CursorType::ResizeAll);
            break;

        case Edge::CornerTopRight:
            m_WindowPosition.y = m_InitialWindowPosition.y + delta.y;
            m_WindowSize.y = std::max(m_InitialWindowSize.y - delta.y, minSize.y);
            m_WindowSize.x = std::max(m_InitialWindowSize.x + delta.x, minSize.x);
            Forge::RenderCommand::SetCursorType(Forge::CursorType::ResizeAll);
            break;

        case Edge::CornerBottomLeft:
            m_WindowSize.y = std::max(m_InitialWindowSize.y + delta.y, minSize.y);
            m_WindowPosition.x = m_InitialWindowPosition.x + delta.x;
            m_WindowSize.x = std::max(m_InitialWindowSize.x - delta.x, minSize.x);
            Forge::RenderCommand::SetCursorType(Forge::CursorType::ResizeAll);
            break;

            // NOTE: Single side
        case Edge::Right:
            m_WindowSize.x = std::max(m_InitialWindowSize.x + delta.x, minSize.x);
            Forge::RenderCommand::SetCursorType(Forge::CursorType::HResize);
            break;

        case Edge::Top:
            m_WindowPosition.y = m_InitialWindowPosition.y + delta.y;
            m_WindowSize.y = std::max(m_InitialWindowSize.y - delta.y, minSize.y);
            Forge::RenderCommand::SetCursorType(Forge::CursorType::VResize);
            break;

        case Edge::Left:
            m_WindowPosition.x = m_InitialWindowPosition.x + delta.x;
            m_WindowSize.x = std::max(m_InitialWindowSize.x - delta.x, minSize.x);
            Forge::RenderCommand::SetCursorType(Forge::CursorType::HResize);
            break;

        case Edge::Bottom:

            m_WindowSize.y = std::max(m_InitialWindowSize.y + delta.y, minSize.y);
            Forge::RenderCommand::SetCursorType(Forge::CursorType::VResize);
            break;

        default:
            // Optionally handle other cases or do nothing
            break;
    }
}
}  // namespace BfUI
