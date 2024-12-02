//
// Created by toor on 2024-11-27.
//

#ifndef DRAWLIST_H
#define DRAWLIST_H

#include <cstdint>
#include <string>
#include <vector>

#include "Types.h"

namespace BfUI {

enum class WidgetElementType : uint32_t
{
    Panel = 1,
    Text = 2,
};

struct DrawListVertex
{
    vec2f Position;
    vec4f Color;
    vec2f TexCoord;
    float TexIndex;
    vec2f Size;
    float WidgetElementType;
};


struct DrawListData
{
    std::vector<uint8_t> Vertices;
    std::vector<uint32_t> Indices;

    DrawListData operator+(const DrawListData& other) const
    {
        DrawListData result;

        // Combine vertices
        result.Vertices = Vertices;
        result.Vertices.insert(result.Vertices.end(), other.Vertices.begin(), other.Vertices.end());

        // Calculate vertex count offset
        uint32_t vertexOffset =
            Vertices.size() /
            sizeof(DrawListVertex);  // Ensure proper vertex count based on struct size

        // Combine indices with offset
        result.Indices = Indices;
        for (uint32_t index : other.Indices)
        {
            result.Indices.push_back(index + vertexOffset);
        }

        return result;
    }
};

class DrawList
{
public:
    static const DrawListData DrawPanel(const vec2i position,
                                        const vec2i size,
                                        const vec4f color,
                                        float const textureIndex = 0);


    static const DrawListData DrawText(const std::string& text,
                                       const vec2i position,
                                       const float scale,
                                       const vec4f color,
                                       const float textureIndex);


    static const DrawListData DrawTextConstraind(
        const std::string& text,
        const vec2i position,
        const float scale,
        const vec4f color,
        const float textureIndex,
        vec2i constrains);
    static const uint32_t GetVerticesSize();

    static const uint32_t GetIndicesSize();

private:
    static uint32_t m_VerticesSize;
    static uint32_t m_IndicesSize;
};

}  // namespace BfUI

#endif
