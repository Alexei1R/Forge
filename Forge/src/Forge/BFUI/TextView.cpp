
#include "TextView.h"

#include "Forge/BFUI/Theme.h"
#include "Forge/BFUI/Types.h"
#include "Forge/Core/Log/Log.h"
#include "Forge/Events/ImplEvent.h"
#include "Forge/Renderer/RenderCommand.h"

namespace bf {

std::shared_ptr<TextView> TextView::Create(const std::string& text) {
    return std::shared_ptr<TextView>(new TextView(text));
}

TextView::TextView(const std::string& text)
    : m_Padding(vec4i(8.0)) {
    m_Position = {4000, 400};
    m_Size = {60, 50};
    m_Text = text;

    // NOTE: Iniditialize With values from theme
    m_ColorBackground = Theme::GetColor(WidgetType::Window, WidgetState::Default);
    Update();
}

void TextView::Update() {
    DrawListData dataStore;
    for (auto& child : m_Children) {
        DrawListData data = child->GetDrawList();
        dataStore += data;
    }
    // NOTE: Occupy the remaining area
    if (m_ParentWidget != nullptr) {
        vec2i parentPos = m_ParentWidget->GetPosition();
        vec2i parentSize = m_ParentWidget->GetSize();
        vec2i parentPadding = m_ParentWidget->GetPadding();

        m_Position.x = parentPos.x + m_Padding.x;
        m_Position.y = parentPos.y + m_Padding.y;
        m_Size.x = parentSize.x - m_Padding.x - parentPadding.x - 25;
        m_Size.y = parentSize.y - m_Padding.y - parentPadding.y - 30;
    }
    float textScale = 24.0f;
    vec2f textSize = DrawList::MeasureText(m_Text, textScale);
    vec2i textPosition;
    textPosition.x = m_Position.x + (m_Size.x / 2) - static_cast<int>(textSize.x / 2);
    textPosition.y = m_Position.y + (m_Size.y / 2) + static_cast<int>(textSize.y / 2);

    DrawListData textData = DrawList::DrawText(m_Text, textPosition, textScale, m_ColorBackground, 0);
    DrawListData panelData = DrawList::DrawPanel(m_Position, m_Size, m_ColorBackground, 0);

    m_DrawListData = dataStore + textData + panelData;
}

std::shared_ptr<Widget> TextView::SetText(const std::string& text) {
    if (m_Text != text) {
        m_Text = text;
        Update();
    }
    return shared_from_this();
}

std::shared_ptr<Widget> TextView::SetPosition(const vec2i& position) {
    if (m_Position != position) {
        m_Position = position;
        Update();
    }
    return shared_from_this();
}

std::shared_ptr<Widget> TextView::SetSize(const vec2i& size) {
    if (m_Size != size) {
        m_Size = size;
        Update();
    }
    return shared_from_this();
}

std::shared_ptr<Widget> TextView::SetPadding(const vec4i& padding) {
    m_Padding = padding;
    return shared_from_this();
};

const DrawListData TextView::GetDrawList() {
    return m_DrawListData;
}

vec2i TextView::GetPosition() const {
    return m_Position;
}
vec2i TextView::GetSize() const {
    return m_Size;
}

vec4i TextView::GetPadding() const {
    return m_Padding;
}

void TextView::SetParent(std::shared_ptr<Widget> parentWidget) {
    LOG_TRACE("SetParent Widget TextView")
    m_ParentWidget = parentWidget;
}

void TextView::AddChild(std::shared_ptr<Widget> child) {
    child->SetParent(shared_from_this());
    m_Children.push_back(child);
    Update();
}

void TextView::OnEvent(const Forge::Event& event) {
    for (auto& child : m_Children) {
        child->OnEvent(event);
    }
}

} // namespace bf
