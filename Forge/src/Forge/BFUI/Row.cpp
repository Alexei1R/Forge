//
// Created by toor on 2024-12-03.
//

#include "Row.h"


namespace BfUI {


std::shared_ptr<Row> Row::Create(std::initializer_list<std::shared_ptr<Widget>> widgets)
{
    auto row = std::shared_ptr<Row>(new Row());
    for (auto& widget : widgets)
    {
        row->AddChild(widget);
    }
    return row;
}

Row::Row()
{
    LOG_WARN("Row Constructor");
    m_Position = {300, 300};
}


void Row::SetParent(std::shared_ptr<Widget> parentWidget)
{
    m_ParentWidget = parentWidget;
}


void Row::AddChild(std::shared_ptr<Widget> child)
{
    try
    {
        child->SetParent(shared_from_this());
        m_Children.push_back(child);
    }
    catch (const std::bad_weak_ptr&)
    {
        LOG_ERROR("Row::AddChild - shared_from_this() failed. Ensure Row is managed by "
                  "shared_ptr.");
        throw;
    }
}

const DrawListData Row::GetDrawList()
{
    DrawListData combinedDrawList;

    if (!m_ParentWidget)
    {
        LOG_ERROR("Row::GetDrawList - Parent widget is null.");
        return combinedDrawList;
    }

    vec2i parentPos = m_ParentWidget->GetPosition();
    vec2i parentSize = m_ParentWidget->GetSize();

    uint32_t advanceX = 0;
    m_Position = parentPos;
    m_Size = {0, 0};

    for (auto& child : m_Children)
    {
        if (!child)
            continue;  // Skip null children

        vec2i childPos = parentPos;
        childPos.x += advanceX;
        childPos.y += 5;  // Padding from the top

        child->SetPosition(childPos);
        combinedDrawList = combinedDrawList + child->GetDrawList();
        advanceX += child->GetSize().x + 5;  // Increment horizontal offset with padding


        m_Position.x -= advanceX;
        m_Size.x += child->GetSize().x;
    }

    return combinedDrawList;
}

void Row::OnEvent(const Forge::Event& event)
{
    for (auto& child : m_Children)
    {
        child->OnEvent(event);
    }
}

vec2i Row::GetPosition() const
{
    return m_Position;
}

vec2i Row::GetSize() const
{
    return m_Size;
}

void Row::SetPosition(const vec2i& position)
{
    m_Position = position;
}

void Row::SetSize(const vec2i& size)
{
    m_Size = size;
}


}  // namespace BfUI
