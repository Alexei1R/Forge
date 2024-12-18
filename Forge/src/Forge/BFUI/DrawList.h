
#ifndef DRAWLIST_H
#define DRAWLIST_H

#include <cstdint>
#include <string>
#include <vector>

#include "Types.h"

namespace bf {

enum class WidgetElementType : uint32_t {
    Panel = 1,
    Text = 2,
};

struct DrawListVertex {
    vec2f Position;
    vec4f Color;
    vec2f TexCoord;
    float TexIndex;
    vec2f Size;
    float WidgetElementType;
};

struct DrawListData {
    std::vector<DrawListVertex> Vertices;
    std::vector<uint32_t> Indices;

    DrawListData operator+(const DrawListData& other) const {
        DrawListData result;

        result.Vertices = Vertices;
        result.Vertices.insert(result.Vertices.end(), other.Vertices.begin(), other.Vertices.end());

        uint32_t vertexOffset = Vertices.size();

        result.Indices = Indices;
        for (uint32_t index : other.Indices) {
            result.Indices.push_back(index + vertexOffset);
        }

        return result;
    }

    DrawListData& operator+=(const DrawListData& other) {
        uint32_t vertexOffset = Vertices.size();

        Vertices.insert(Vertices.end(), other.Vertices.begin(), other.Vertices.end());

        for (uint32_t index : other.Indices) {
            Indices.push_back(index + vertexOffset);
        }

        return *this;
    }
};

class DrawList {
public:
    static const DrawListData DrawPanel(const vec2i position, const vec2i size, const vec4f color, float const textureIndex = 0);

    static const DrawListData DrawText(const std::string& text, const vec2i position, const float scale, const vec4f color,
                                       const float textureIndex);

    static vec2f MeasureText(const std::string& text, float scale);

    static const uint32_t GetVerticesSize();

    static const uint32_t GetIndicesSize();

private:
    static uint32_t m_VerticesSize;
    static uint32_t m_IndicesSize;
};

} // namespace bf

#endif
