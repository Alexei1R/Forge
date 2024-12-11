
#include "Button.h"
#include "Forge/BFUI/Theme.h"
#include "Forge/BFUI/Types.h"
#include "Forge/Core/Log/Log.h"
#include "Forge/Events/ImplEvent.h"
#include "Forge/Renderer/RenderCommand.h"

namespace bf {

std::shared_ptr<Button> Button::Create(const vec2i& size, const std::string& label)
{
    return std::shared_ptr<Button>(new Button(size, label));
}


Button::Button(const vec2i& size, const std::string& label) :
    m_Name(label), m_MinSize(30, 30), m_Size(size), m_Padding(vec4i(8.0))
{
    m_Position = {50, 50};

    // NOTE: Iniditialize With values from theme
    m_ColorBackground = Theme::GetColor(WidgetType::Button, WidgetState::Default);
    m_ColorBackgroundDefault = Theme::GetColor(WidgetType::Button, WidgetState::Default);
    m_ColorBackgroundHover = Theme::GetColor(WidgetType::Button, WidgetState::Hover);
    m_ColorBackgroundPressed = Theme::GetColor(WidgetType::Button, WidgetState::Pressed);
    Update();
}


void Button::Update()
{
    DrawListData dataStore;
    for (auto& child : m_Children)
    {
        DrawListData data = child->GetDrawList();
        dataStore += data;
    }

    float textScale = 24.0f;
    vec2f textSize = DrawList::MeasureText(m_Name, textScale);

    vec2i textPosition;
    textPosition.x = m_Position.x + (m_Size.x / 2) - static_cast<int>(textSize.x / 2);
    textPosition.y = m_Position.y + (m_Size.y / 2) + static_cast<int>(textSize.y / 2);

    DrawListData buttonDrawListData =
        DrawList::DrawText(m_Name, textPosition, textScale, m_ColorBackground, 0);

    DrawListData panelData = DrawList::DrawPanel(m_Position, m_Size, m_ColorBackground, 0);

    buttonDrawListData += panelData;
    m_DrawListData = dataStore + buttonDrawListData;
}

std::shared_ptr<Widget> Button::SubscribeEvents(EventCallback callback)
{
    m_EventCallback = callback;
    return shared_from_this();
}

std::shared_ptr<Widget> Button::SetText(const std::string& label)
{
    if (m_Name != label)
    {
        m_Name = label;
        Update();
    }
    return shared_from_this();
}


std::shared_ptr<Widget> Button::SetPosition(const vec2i& position)
{
    if (m_Position != position)
    {
        m_Position = position;
        Update();
    }
    return shared_from_this();
}

std::shared_ptr<Widget> Button::SetSize(const vec2i& size)
{
    if (m_Size != size)
    {
        m_Size = size;
        Update();
    }

    return shared_from_this();
}


std::shared_ptr<Widget> Button::SetPadding(const vec4i& padding)
{
    m_Padding = padding;

    return shared_from_this();
};

const DrawListData Button::GetDrawList()
{
    return m_DrawListData;
}

vec2i Button::GetPosition() const
{
    return m_Position;
}
vec2i Button::GetSize() const
{
    return m_Size;
}

vec4i Button::GetPadding() const
{
    return m_Padding;
}


void Button::SetParent(std::shared_ptr<Widget> parentWidget)
{
    m_ParentWidget = parentWidget;
}

void Button::AddChild(std::shared_ptr<Widget> child)
{
    child->SetParent(shared_from_this());
    m_Children.push_back(child);
    Update();
}

void Button::OnEvent(const Forge::Event& event)
{
    for (auto& child : m_Children)
    {
        child->OnEvent(event);
    }

    auto [x, y] = Forge::Mouse::GetMousePosition();
    glm::vec2 mousePos(x, y);


    if (event.GetType() == Forge::EventType::Key)
    {
        const auto& keyEvent = static_cast<const Forge::KeyEvent&>(event);
        if (keyEvent.GetAction() == Forge::Action::MousePress)
        {
            if (IsInBounds(mousePos))
            {
                m_IsPressed = true;
                if (m_EventCallback)
                {
                    m_EventCallback(WidgetEvent::ButtonPress, *this);
                }
                Update();
                m_ColorBackground = m_ColorBackgroundPressed;
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

                Update();
                m_ColorBackground = m_ColorBackgroundDefault;
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
                    Update();
                    m_ColorBackground = m_ColorBackgroundHover;
                }
            }
            else if (!m_IsHovered && wasHovered && !m_IsPressed)
            {
                Forge::RenderCommand::SetCursorType(Forge::CursorType::Arrow);

                Update();
                m_ColorBackground = m_ColorBackgroundDefault;
            }
        }
    }
}


const bool Button::IsInBounds(const glm::vec2& point) const
{
    return point.x >= m_Position.x && point.x <= m_Position.x + m_Size.x &&
           point.y >= m_Position.y && point.y <= m_Position.y + m_Size.y;
}


}  // namespace bf
