//
// Created by toor on 2024-11-21.
//


#include "Panel.h"
#include "glm/fwd.hpp"
#include <cstring>


namespace BfUI {


Panel::Panel(glm::vec2 position, glm::vec2 size) : m_Position(position), m_Size(size)
{
    UpdateBounds();
}

void Panel::UpdateBounds()
{
    m_TopLeft = m_Position;
    m_BottomRight = m_Position + m_Size;
}

glm::vec2 Panel::GetTopLeft() const
{
    return m_TopLeft;
}

glm::vec2 Panel::GetBottomRight() const
{
    return m_BottomRight;
}

glm::vec2 Panel::GetCenter() const
{
    return m_Position + (m_Size * 0.5f);
}


bool Panel::ContainsPoint(const glm::vec2& point) const
{
    return (point.x >= m_TopLeft.x && point.x <= m_BottomRight.x && point.y >= m_TopLeft.y &&
            point.y <= m_BottomRight.y);
}

bool Panel::OverlapAnotherPanel(const Panel& other) const
{
    return !(other.m_TopLeft.x > m_BottomRight.x || other.m_BottomRight.x < m_TopLeft.x ||
             other.m_TopLeft.y > m_BottomRight.y || other.m_BottomRight.y < m_TopLeft.y);
}

const std::vector<PanelVertex>& Panel::GetVertices() const
{
    glm::vec2 topLeft = m_TopLeft;
    glm::vec2 bottomRight = m_BottomRight;
    glm::vec2 topRight = glm::vec2(bottomRight.x, topLeft.y);
    glm::vec2 bottomLeft = glm::vec2(topLeft.x, bottomRight.y);


    static std::vector<PanelVertex> vertices = {
        {glm::vec2(topLeft)},
        {glm::vec2(topRight)},
        {glm::vec2(bottomRight)},
        {glm::vec2(bottomLeft)}};


    return vertices;
}

const std::vector<uint32_t>& Panel::GetIndices() const
{
    static std::vector<uint32_t> indices = {0, 1, 2, 2, 3, 0};
    return indices;
}


}  // namespace BfUI
