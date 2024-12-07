//
// Created by toor on 2024-12-03.
//

#include "Column.h"


#include "Forge/BFUI/Row.h"
#include "Forge/Core/Log/Log.h"

namespace BfUI {


std::shared_ptr<Column> Column::Create(std::initializer_list<std::shared_ptr<Widget>> widgets)
{
    auto row = std::shared_ptr<Column>(new Column());
    for (auto& widget : widgets)
    {
        row->AddChild(widget);
    }
    return row;
}

Column::Column()
{
    LOG_WARN("Column Constructor");
    m_Position = {50, 50};
}


void Column::SetParent(std::shared_ptr<Widget> parentWidget)
{
    m_ParentWidget = parentWidget;
}

void Column::AddChild(std::shared_ptr<Widget> child)
{
    child->SetParent(shared_from_this());
    m_Children.push_back(child);
}


const DrawListData Column::GetDrawList()
{
    DrawListData combinedDrawList;

    if (!m_ParentWidget)
    {
        LOG_ERROR("Column::GetDrawList - Parent widget is null.");
        return combinedDrawList;
    }

    vec2i parentPos = m_ParentWidget->GetPosition();
    vec2i parentSize = m_ParentWidget->GetSize();

    m_Position.x = parentPos.x;
    m_Position.y = parentPos.y + parentSize.y;
    m_Size = {0, 0};

    uint32_t advanceY = 0;

    for (auto& child : m_Children)
    {
        if (!child)
            continue;  // Skip null children

        vec2i childPos;
        childPos.x = parentPos.x + 5;  // Padding from the left
        childPos.y = parentPos.y + advanceY + 5;  // Padding from the top

        child->SetPosition(childPos);
        combinedDrawList = combinedDrawList + child->GetDrawList();
        advanceY += child->GetSize().y + 5;  // Increment vertical offset with padding
        m_Position.y -= advanceY;
        m_Size.y += child->GetSize().y;
    }

    return combinedDrawList;
}

void Column::OnEvent(const Forge::Event& event)
{
    for (auto& child : m_Children)
    {
        child->OnEvent(event);
    }
}

vec2i Column::GetPosition() const
{
    return m_Position;
}

vec2i Column::GetSize() const
{
    return m_Size;
}

void Column::SetPosition(const vec2i& position)
{
    m_Position = position;
}

void Column::SetSize(const vec2i& size)
{
    m_Size = size;
}


}  // namespace BfUI
