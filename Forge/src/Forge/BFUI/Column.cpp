

#include "Column.h"


namespace bf {


std::shared_ptr<Column> Column::Create(std::initializer_list<std::shared_ptr<Widget>> widgets)
{
    auto row = std::shared_ptr<Column>(new Column());
    for (auto& widget : widgets)
    {
        row->AddChild(widget);
    }
    return row;
}

Column::Column() : m_Padding(vec4i(0))
{
    m_Position = {300, 300};
    m_Size = {333, 333};
}


void Column::SetParent(std::shared_ptr<Widget> parentWidget)
{
    m_ParentWidget = parentWidget;
}


void Column::AddChild(std::shared_ptr<Widget> child)
{
    try
    {
        child->SetParent(shared_from_this());
        m_Children.push_back(child);
    }
    catch (const std::bad_weak_ptr&)
    {
        LOG_ERROR("Column::AddChild - shared_from_this() failed. Ensure Column is managed by "
                  "shared_ptr.");
        throw;
    }
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

    int advanceY = 0;  // Track vertical placement
    m_Position = parentPos;
    m_Size = {0, 0};

    for (auto& child : m_Children)
    {
        if (!child)
            continue;  // Skip null children

        // Retrieve padding: x (left), y (top), z (right), w (bottom)
        glm::vec4 padding = child->GetPadding();

        vec2i childPos = parentPos;

        // Apply horizontal and vertical padding using glm components
        childPos.x += static_cast<int>(padding.x);  // padding.x = left
        childPos.y += advanceY + static_cast<int>(padding.y);  // padding.y = top

        // Set the child's position
        child->SetPosition(childPos);

        // Combine the child's draw list
        combinedDrawList = combinedDrawList + child->GetDrawList();

        // Update advanceY with child's height and vertical padding
        advanceY += child->GetSize().y + static_cast<int>(padding.y) +
                    static_cast<int>(padding.w);  // padding.w = bottom

        // Update m_Size.y to the total height
        m_Size.y = advanceY;

        // Update m_Size.x to the maximum width among children
        int childTotalWidth = child->GetSize().x + static_cast<int>(padding.x) +
                              static_cast<int>(padding.z);  // padding.x = left, padding.z = right
        m_Size.x = std::max(m_Size.x, childTotalWidth);
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
    vec2i returnPos = m_Position;
    returnPos.y += m_Size.y;
    return returnPos;
}

vec2i Column::GetSize() const
{
    return m_Size;
}

vec4i Column::GetPadding() const
{
    return m_Padding;
}


std::shared_ptr<Widget> Column::SetPosition(const vec2i& position)
{
    if (m_Position != position)
    {
        m_Position = position;
    }
    return shared_from_this();
}

std::shared_ptr<Widget> Column::SetSize(const vec2i& size)
{
    if (m_Size != size)
    {
        m_Size = size;
    }

    return shared_from_this();
}


std::shared_ptr<Widget> Column::SetPadding(const vec4i& padding)
{
    m_Padding = padding;

    return shared_from_this();
};


}  // namespace bf
