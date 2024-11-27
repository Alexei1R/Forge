//
// Created by toor on 2024-11-27.
//

#ifndef DRAWLIST_H
#define DRAWLIST_H

#include "Forge/BFUI/Widget.h"
#include <cstdint>
#include <utility>
#include <vector>

#include "Types.h"

namespace BfUI {

struct DrawListVertex
{
    glm::vec3 Position;
    glm::vec4 Color;
    glm::vec2 TexCoord;
    float TexIndex;
    float ElementType;
};


class DrawList
{
public:
    /*static void AddWidget(const std::shared_ptr<BfUI::Widget>& widget);*/

    static std::vector<std::pair<DrawListVertex, uint32_t>> DrawPanel(vec3i position, vec3i size);

private:
    static uint32_t m_VerticesSize;
    static uint32_t m_IndicesSize;
};


}  // namespace BfUI


#endif
