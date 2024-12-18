
#include "Window.h"

#include "Forge/BFUI/Theme.h"
#include "Forge/BFUI/Types.h"
#include "Forge/Core/Log/Log.h"
#include "Forge/Events/ImplEvent.h"
#include "Forge/Events/KeyCodes.h"
#include "Forge/Renderer/RenderCommand.h"

namespace bf {

std::shared_ptr<Window> Window::Create(const std::string& label) {
    return std::shared_ptr<Window>(new Window(label));
}

Window::Window(const std::string& label)
    : m_Name(label)
    , m_MinSize(30, 30)
    , m_Padding(vec4i(0)) {
    m_Position = {250, 250};
    m_Size = {500, 500};

    // NOTE: Set DefaultWindowColor
    m_ColorBackground = Theme::GetColor(WidgetType::Window, WidgetState::Default);
    Update();
}

void Window::Update() {
    DrawListData dataStore;
    for (auto& child : m_Children) {
        DrawListData data = child->GetDrawList();

        dataStore += data;
    }
    m_DrawListData = dataStore + DrawList::DrawPanel(m_Position, m_Size, m_ColorBackground);

    LOG_ERROR("compute drawlist")
}

const DrawListData Window::GetDrawList() {
    LOG_ERROR("return DrawList")
    return m_DrawListData;
}

vec2i Window::GetPosition() const {
    return m_Position;
}
vec2i Window::GetSize() const {
    return m_Size;
}
std::shared_ptr<Widget> Window::SetPosition(const vec2i& position) {
    if (m_Position != position) {
        m_Position = position;
        Update();
    }
    return shared_from_this();
}

std::shared_ptr<Widget> Window::SetSize(const vec2i& size) {
    if (m_Size != size) {
        m_Size = size;
        Update();
    }

    return shared_from_this();
}

std::shared_ptr<Widget> Window::SetPadding(const vec4i& padding) {
    m_Padding = padding;

    return shared_from_this();
};

vec4i Window::GetPadding() const {
    return m_Padding;
}

void Window::SetParent(std::shared_ptr<Widget> parentWidget) {
    m_ParentWidget = parentWidget;
}

void Window::AddChild(std::shared_ptr<Widget> child) {
    child->SetParent(shared_from_this());
    m_Children.push_back(child);
    Update();
}

void Window::OnEvent(const Forge::Event& event) {
    for (auto& child : m_Children) {
        child->OnEvent(event);
    }
    auto [x, y] = Forge::Mouse::GetMousePosition();

    glm::vec2 mousePos(x, y);

    if (event.GetType() == Forge::EventType::Key) {
        const auto& keyEvent = static_cast<const Forge::KeyEvent&>(event);

        if (keyEvent.GetAction() == Forge::Action::MousePress && keyEvent.GetKey() == Forge::Key::LeftMouse) {
            selectedEdge = IsNearEdge(m_Position, m_Size);
            if (selectedEdge != (Edge)0) {
                m_InitialPosition = m_Position;
                m_InitialSize = m_Size;
                m_InitialMousePos = mousePos;
                m_IsAnyEdgeSelected = true;
            }
        }

        else if (keyEvent.GetAction() == Forge::Action::MousePress && keyEvent.GetKey() == Forge::Key::RightMouse) {
            selectedEdge = IsNearEdge(m_Position, m_Size, 20);
            if (selectedEdge == Edge::CornerTopLeft) {
                m_InitialPosition = m_Position;
                m_InitialSize = m_Size;
                m_InitialMousePos = mousePos;
                m_IsWindowDragged = true;
            }
        } else if (keyEvent.GetAction() == Forge::Action::MouseRelease) {
            if (m_IsAnyEdgeSelected) {
                Forge::RenderCommand::SetCursorType(Forge::CursorType::Arrow);
                m_IsAnyEdgeSelected = false;
            }

            if (m_IsWindowDragged) {
                Forge::RenderCommand::SetCursorType(Forge::CursorType::Arrow);
                m_IsWindowDragged = false;
            }
        }
    } else if (event.GetType() == Forge::EventType::Mouse) {
        const auto& mouseEvent = static_cast<const Forge::MouseEvent&>(event);

        // Handle mouse move for hover
        if (mouseEvent.GetAction() == Forge::Action::MouseMove) {
            if (m_IsAnyEdgeSelected) {
                glm::vec2 delta;
                delta.x = mousePos.x - m_InitialMousePos.x;
                delta.y = mousePos.y - m_InitialMousePos.y;
                HandleResize(delta, selectedEdge);
                Update();
            }

            if (m_IsWindowDragged) {
                glm::vec2 delta;
                delta.x = mousePos.x - m_InitialMousePos.x;
                delta.y = mousePos.y - m_InitialMousePos.y;
                HandleDragg(delta);
                Update();
            }
        }
    }
}

const bool Window::IsInBounds(const glm::vec2& point) const {
    return point.x >= m_Position.x && point.x <= m_Position.x + m_Size.x && point.y >= m_Position.y &&
           point.y <= m_Position.y + m_Size.y;
}

const Edge Window::IsNearEdge(const vec2i& position, const vec2i& size, const int threshold) const {
    vec2i topLeft = position;
    vec2i bottomRight = topLeft + size;

    // Get mouse position
    auto [mouseX, mouseY] = Forge::Mouse::GetMousePosition();
    glm::vec2 mousePos(mouseX, mouseY);

    float left = topLeft.x;
    float right = bottomRight.x;
    float top = topLeft.y;
    float bottom = bottomRight.y;

    bool nearLeft = (std::abs(mousePos.x - left) <= threshold) && (mousePos.y <= bottom) && (mousePos.y >= top);
    bool nearRight = (std::abs(mousePos.x - right) <= threshold) && (mousePos.y <= bottom) && (mousePos.y >= top);
    bool nearBottom = (std::abs(mousePos.y - bottom) <= threshold) && (mousePos.x >= left) && (mousePos.x <= right);
    bool nearTop = (std::abs(mousePos.y - top) <= threshold) && (mousePos.x >= left) && (mousePos.x <= right);

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

    return Edge(0);
}

void Window::HandleResize(const vec2i& delta, Edge selectedEdge) {
    switch (selectedEdge) {
    case Edge::CornerTopLeft:
        m_Position.x = m_InitialPosition.x + delta.x;
        m_Position.y = m_InitialPosition.y + delta.y;
        m_Size.x = std::max(m_InitialSize.x - delta.x, m_MinSize.x);
        m_Size.y = std::max(m_InitialSize.y - delta.y, m_MinSize.y);
        Forge::RenderCommand::SetCursorType(Forge::CursorType::ResizeAll);
        break;

    case Edge::CornerBottomRight:
        m_Size.x = std::max(m_InitialSize.x + delta.x, m_MinSize.x);
        m_Size.y = std::max(m_InitialSize.y + delta.y, m_MinSize.y);
        Forge::RenderCommand::SetCursorType(Forge::CursorType::ResizeAll);
        break;

    case Edge::CornerTopRight:
        m_Position.y = m_InitialPosition.y + delta.y;
        m_Size.y = std::max(m_InitialSize.y - delta.y, m_MinSize.y);
        m_Size.x = std::max(m_InitialSize.x + delta.x, m_MinSize.x);
        Forge::RenderCommand::SetCursorType(Forge::CursorType::ResizeAll);
        break;

    case Edge::CornerBottomLeft:
        m_Size.y = std::max(m_InitialSize.y + delta.y, m_MinSize.y);
        m_Position.x = m_InitialPosition.x + delta.x;
        m_Size.x = std::max(m_InitialSize.x - delta.x, m_MinSize.x);
        Forge::RenderCommand::SetCursorType(Forge::CursorType::ResizeAll);
        break;

        // NOTE: Single side
    case Edge::Right:
        m_Size.x = std::max(m_InitialSize.x + delta.x, m_MinSize.x);
        Forge::RenderCommand::SetCursorType(Forge::CursorType::HResize);
        break;

    case Edge::Top:
        m_Position.y = m_InitialPosition.y + delta.y;
        m_Size.y = std::max(m_InitialSize.y - delta.y, m_MinSize.y);
        Forge::RenderCommand::SetCursorType(Forge::CursorType::VResize);
        break;

    case Edge::Left:
        m_Position.x = m_InitialPosition.x + delta.x;
        m_Size.x = std::max(m_InitialSize.x - delta.x, m_MinSize.x);
        Forge::RenderCommand::SetCursorType(Forge::CursorType::HResize);
        break;

    case Edge::Bottom:

        m_Size.y = std::max(m_InitialSize.y + delta.y, m_MinSize.y);
        Forge::RenderCommand::SetCursorType(Forge::CursorType::VResize);
        break;

    default:
        // Optionally handle other cases or do nothing
        break;
    }
}

void Window::HandleDragg(const vec2i& delta) {
    m_Position.x = m_InitialPosition.x + delta.x;
    m_Position.y = m_InitialPosition.y + delta.y;
}
} // namespace bf
