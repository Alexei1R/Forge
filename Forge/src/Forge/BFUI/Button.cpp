//
// Created by toor on 2024-11-28.
//

#include "Button.h"
#include "Forge/BFUI/DrawList.h"
#include "Forge/BFUI/Theme.h"
#include "Forge/BFUI/Types.h"
#include "Forge/Core/Log/Log.h"
#include "Forge/Events/ImplEvent.h"
#include "Forge/Renderer/RenderCommand.h"
#include "WidgetStack.h"
#include <algorithm>
#include <memory>
#include "Forge/Renderer/Material.h"
#include "Forge/Renderer/ShaderManager.h"
#include "Forge/Renderer/Font.h"

namespace BfUI {


std::shared_ptr<Button> Button::Create(const vec2i& size, const std::string& label)
{
    return std::shared_ptr<Button>(new Button(size, label));
}


Button::Button(const vec2i& size, const std::string& label) :
    m_BtnPosition({50, 50}), m_BtnSize(size), m_BtnLabel(label)

{
    // NOTE: Iniditialize With values from theme
    m_ColorBackground = Theme::GetColor(WidgetType::Button, WidgetState::Default);
    m_ColorBackgroundDefault = Theme::GetColor(WidgetType::Button, WidgetState::Default);
    m_ColorBackgroundHover = Theme::GetColor(WidgetType::Button, WidgetState::Hover);
    m_ColorBackgroundPressed = Theme::GetColor(WidgetType::Button, WidgetState::Pressed);
    Update();
}


Button::~Button() {}


void Button::Update()
{
    auto& panelData = DrawList::DrawPanel(m_BtnPosition, m_BtnSize, m_ColorBackground);
    auto& textData =
        DrawList::DrawText(m_BtnLabel, m_BtnPosition + (m_BtnSize / 5), 24, m_ColorBackground, 0);
    m_DrawListData = textData + panelData;
}


void Button::SetParent(std::shared_ptr<Widget> parentWidget)
{
    m_ParentWidget = parentWidget;
}

vec2i Button::GetPosition() const
{
    return m_BtnPosition;
}
vec2i Button::GetSize() const
{
    return m_BtnSize;
}
void Button::SetPosition(const vec2i& position)
{
    if (m_BtnPosition != position)
    {
        m_BtnPosition = position;
        Update();
    }
}
void Button::SetSize(const vec2i& size)
{
    if (m_BtnSize != size)
    {
        m_BtnSize = size;
        Update();
    }
}


const DrawListData Button::GetDrawList()
{
    return m_DrawListData;
};


const bool Button::IsInBounds(const glm::vec2& point) const
{
    vec2i m_BottomLeft = m_BtnPosition;
    vec2i m_TopRight = m_BtnPosition + m_BtnSize;
    vec2i m_TopLeft = vec2i(m_BottomLeft.x, m_TopRight.y);
    vec2i m_BottomRight = vec2i(m_TopRight.x, m_BottomLeft.y);

    return (point.x >= m_BottomLeft.x && point.x <= m_TopRight.x && point.y >= m_BottomLeft.y &&
            point.y <= m_TopRight.y);
}


void Button::Move(vec2i position)
{
    m_BtnPosition = position;
    Update();
}


void Button::SubscribeEvents(EventCallback callback)
{
    m_EventCallback = callback;
}

void Button::OnEvent(const Forge::Event& event)
{
    auto [x, y] = Forge::Mouse::GetMousePosition();
    auto [appX, appY] = Forge::ApplicationStats::GetApplicationSize();

    // Convert mouse coordinates to match your coordinate system
    glm::vec2 mousePos(x, appY - y);  // Flip Y coordinate if needed

    if (event.GetType() == Forge::EventType::Key)
    {
        const auto& keyEvent = static_cast<const Forge::KeyEvent&>(event);

        // Handle mouse press
        if (keyEvent.GetAction() == Forge::Action::MousePress)
        {
            if (IsInBounds(mousePos))
            {
                m_IsPressed = true;
                if (m_EventCallback)
                {
                    m_EventCallback(WidgetEvent::ButtonPress, *this);
                }
                m_ColorBackground = m_ColorBackgroundPressed;
                Update();
            }
        }
        // Handle mouse release
        else if (keyEvent.GetAction() == Forge::Action::MouseRelease)
        {
            if (m_IsPressed)
            {
                m_IsPressed = false;
                if (m_EventCallback)
                {
                    m_EventCallback(WidgetEvent::ButtonRelease, *this);
                }

                m_ColorBackground = m_ColorBackgroundDefault;
                Update();
            }
        }
    }
    else if (event.GetType() == Forge::EventType::Mouse)
    {
        const auto& mouseEvent = static_cast<const Forge::MouseEvent&>(event);

        // Handle mouse move for hover
        if (mouseEvent.GetAction() == Forge::Action::MouseMove)
        {
            bool wasHovered = m_IsHovered;
            m_IsHovered = IsInBounds(mousePos);

            if (m_IsHovered && !wasHovered)
            {
                if (m_EventCallback)
                {
                    m_EventCallback(WidgetEvent::ButtonHover, *this);
                    Forge::RenderCommand::SetCursorType(Forge::CursorType::Hand);
                }
                if (!m_IsPressed)
                {
                    m_ColorBackground = m_ColorBackgroundHover;
                    Update();
                }
            }
            else if (!m_IsHovered && wasHovered && !m_IsPressed)
            {
                Forge::RenderCommand::SetCursorType(Forge::CursorType::Arrow);

                m_ColorBackground = m_ColorBackgroundDefault;
                Update();
            }
        }
    }
}

}  // namespace BfUI
