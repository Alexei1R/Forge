//
// Created by toor on 2024-11-21.
//

#include "Button.h"
#include "Forge/BFUI/Theme.h"
#include "Forge/Events/ImplEvent.h"

#include <cstring>

namespace BfUI {

std::shared_ptr<Button>
    Button::Create(const glm::vec2& position, const glm::vec2& size, const std::string& label)
{
    auto button = std::shared_ptr<Button>(new Button(position, size, label));
    WidgetStack::AddWidget(button);  // Auto-register with WidgetStack
    return button;
}

Button::Button(const glm::vec2& position, const glm::vec2& size, const std::string& label) :
    m_Position(position), m_Size(size)
{
    m_BottomLeft = m_Position;
    m_TopRight = m_Position + m_Size;
    m_TopLeft = {m_BottomLeft.x, m_TopRight.y};
    m_BottomRight = {m_TopRight.x, m_BottomLeft.y};

    auto& shaderManager = Forge::ShaderManager::GetInstance();
    Handle shaderHandle = shaderManager.LoadShader(Forge::ShaderLayout {
        {Forge::ShaderType::VERTEX, "Assets/Shaders/UI/Button.vert"},
        {Forge::ShaderType::FRAGMENT, "Assets/Shaders/UI/Button.frag"}});

    if (shaderHandle.GetValue() == 0)
    {
        LOG_CRITICAL("Failed to load shader.");
    }

    m_ButtonMaterial = std::make_shared<Forge::Material>(shaderHandle);

    // Set default colors from Theme
    m_DefaultColor = Theme::GetColor(WidgetType::Button, WidgetState::Default);
    m_HoverColor = Theme::GetColor(WidgetType::Button, WidgetState::Hover);
    m_ButtonMaterial->Color = m_DefaultColor;

    m_ButtonMaterial->SetParameter("Color", m_ButtonMaterial->Color);
    m_ButtonMaterial->SetParameter("QuadSize", m_Size);

    // Set rounding from Theme
    float radius = Theme::GetRounding(WidgetType::Button);
    m_ButtonMaterial->SetParameter("Radius", radius);

    float feather = 1.0f;  // Smooth edge width in pixels
    m_ButtonMaterial->SetParameter("Feather", feather);

    m_ButtonBufferLayout = {{Forge::BufferDataType::Float2, "a_Position"},
                            {Forge::BufferDataType::Float2, "a_TexCoord"}};


    UpdateDrawData();
}

Button::~Button() {}

void Button::SubscribeEvents(EventCallback callback)
{
    m_EventCallback = callback;
}

const std::vector<uint8_t>& Button::GetVertices() const
{
    return m_VerticesBytes;
}

const std::vector<uint32_t>& Button::GetIndices() const
{
    return m_Indices;
}

const std::shared_ptr<Forge::Material>& Button::GetMaterial() const
{
    return m_ButtonMaterial;
}

const Forge::BufferLayout& Button::GetLayout() const
{
    return m_ButtonBufferLayout;
}


void Button::Move(const glm::vec2& position)
{
    if (m_Position == position)
    {
        return;
    }
    // Update position and size
    m_Position = position;

    // Update corners based on new position and size
    m_BottomLeft = m_Position;
    m_TopRight = m_Position + m_Size;
    m_TopLeft = {m_BottomLeft.x, m_TopRight.y};
    m_BottomRight = {m_TopRight.x, m_BottomLeft.y};

    // Update draw data for the button
    UpdateDrawData();
}

Button& Button::SetBackgroundColor(const glm::vec4& color)
{
    m_ButtonMaterial->Color = color;
    m_ButtonMaterial->SetParameter("Color", color);
    return *this;
}

bool Button::IsInBounds(const glm::vec2& point) const
{
    return (point.x >= m_BottomLeft.x && point.x <= m_TopRight.x && point.y >= m_BottomLeft.y &&
            point.y <= m_TopRight.y);
}

Forge::EventType Button::GetType() const
{
    return Forge::EventType::Button;
}

Forge::Action Button::GetAction() const
{
    return Forge::Action::UIEvent;
}

std::string Button::ToString() const
{
    return "Button";
}

void Button::OnEvent(const Event& event)
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
                    m_EventCallback(WidgetEvents::ButtonPress, *this);
                }
                SetBackgroundColor(Theme::GetColor(WidgetType::Button, WidgetState::Pressed));
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
                    m_EventCallback(WidgetEvents::ButtonRelease, *this);
                }
                SetBackgroundColor(Theme::GetColor(WidgetType::Button, WidgetState::Default));
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
                    m_EventCallback(WidgetEvents::ButtonHover, *this);
                }
                if (!m_IsPressed)
                {
                    SetBackgroundColor(Theme::GetColor(WidgetType::Button, WidgetState::Hover));
                }
            }
            else if (!m_IsHovered && wasHovered && !m_IsPressed)
            {
                SetBackgroundColor(Theme::GetColor(WidgetType::Button, WidgetState::Default));
            }
        }
    }
}

void Button::UpdateDrawData()
{
    m_TopLeft = {m_BottomLeft.x, m_TopRight.y};
    m_BottomRight = {m_TopRight.x, m_BottomLeft.y};

    std::vector<ButtonVertex> vertices = {
        {m_TopLeft, {0.0f, 1.0f}},
        {m_TopRight, {1.0f, 1.0f}},
        {m_BottomRight, {1.0f, 0.0f}},
        {m_BottomLeft, {0.0f, 0.0f}}};

    m_VerticesBytes.resize(vertices.size() * sizeof(ButtonVertex));
    std::memcpy(m_VerticesBytes.data(), vertices.data(), m_VerticesBytes.size());

    m_Indices = {0, 1, 2, 2, 3, 0};
}

}  // namespace BfUI
