//
// Created by toor on 2024-11-21.
//

#ifndef PANEL_H
#define PANEL_H

#include "glm/glm.hpp"

namespace BfUI {


struct PanelVertex
{
    glm::vec2 Position;
};

class Panel
{
public:
    Panel(glm::vec2 position, glm::vec2 size);

    const std::vector<PanelVertex>& GetVertices() const;
    const std::vector<uint32_t>& GetIndices() const;


    glm::vec2 GetTopLeft() const;
    glm::vec2 GetBottomRight() const;
    glm::vec2 GetCenter() const;

    bool ContainsPoint(const glm::vec2& point) const;
    bool OverlapAnotherPanel(const Panel& other) const;

private:
    void UpdateBounds();

private:
    glm::vec2 m_Position;
    glm::vec2 m_Size;

    glm::vec2 m_TopLeft;
    glm::vec2 m_BottomRight;
};
}  // namespace BfUI


#endif
