
#include "Row.h"

namespace bf {

std::shared_ptr<Row> Row::Create(std::initializer_list<std::shared_ptr<Widget>> widgets) {
    auto row = std::shared_ptr<Row>(new Row());
    for (auto& widget : widgets) {
        row->AddChild(widget);
    }
    return row;
}

Row::Row()
    : m_Padding(vec4i(0)) {
    m_Position = {300, 300};
}

void Row::SetParent(std::shared_ptr<Widget> parentWidget) {
    m_ParentWidget = parentWidget;
}

void Row::AddChild(std::shared_ptr<Widget> child) {
    try {
        child->SetParent(shared_from_this());
        m_Children.push_back(child);
    } catch (const std::bad_weak_ptr&) {
        LOG_ERROR("Row::AddChild - shared_from_this() failed. Ensure Row is managed by "
                  "shared_ptr.");
        throw;
    }
}

const DrawListData Row::GetDrawList() {
    DrawListData combinedDrawList;

    if (!m_ParentWidget) {
        LOG_ERROR("Row::GetDrawList - Parent widget is null.");
        return combinedDrawList;
    }

    vec2i parentPos = m_ParentWidget->GetPosition();
    vec2i parentSize = m_ParentWidget->GetSize();

    uint32_t advanceX = 0;
    m_Position = parentPos;
    m_Size = {0, 0};

    for (auto& child : m_Children) {
        if (!child)
            continue; // Skip null children

        // Retrieve padding: x (left), y (top), z (right), w (bottom)
        glm::vec4 padding = child->GetPadding();

        vec2i childPos = parentPos;

        childPos.x += advanceX + static_cast<int>(padding.x);
        childPos.y += static_cast<int>(padding.y);

        child->SetPosition(childPos);
        combinedDrawList = combinedDrawList + child->GetDrawList();
        advanceX += child->GetSize().x + static_cast<int>(padding.x) + static_cast<int>(padding.z);

        m_Size.x = advanceX;

        int childTotalHeight = child->GetSize().y + static_cast<int>(padding.y) + static_cast<int>(padding.w); // padding.w = bottom
        m_Size.y = std::max(m_Size.y, childTotalHeight);
    }

    return combinedDrawList;
}

void Row::OnEvent(const Forge::Event& event) {
    for (auto& child : m_Children) {
        child->OnEvent(event);
    }
}

vec2i Row::GetPosition() const {
    vec2i returnPos = m_Position;
    returnPos.x += m_Size.x;
    return returnPos;
}

vec2i Row::GetSize() const {
    return m_Size;
}

vec4i Row::GetPadding() const {
    return m_Padding;
}

std::shared_ptr<Widget> Row::SetPosition(const vec2i& position) {
    if (m_Position != position) {
        m_Position = position;
    }
    return shared_from_this();
}

std::shared_ptr<Widget> Row::SetSize(const vec2i& size) {
    if (m_Size != size) {
        m_Size = size;
    }

    return shared_from_this();
}

std::shared_ptr<Widget> Row::SetPadding(const vec4i& padding) {
    m_Padding = padding;

    return shared_from_this();
};

} // namespace bf
