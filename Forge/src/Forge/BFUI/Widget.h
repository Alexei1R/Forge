//
// Created by toor on 2024-11-21.
//

#ifndef WIDGET_H
#define WIDGET_H


#include <cstdint>
#include <vector>


#include "Forge/BFUI/DrawList.h"
#include "Forge/Events/Event.h"
#include "Forge/Renderer/Buffer/Buffer.h"
#include "Forge/Renderer/Material.h"
#include "Forge/Renderer/RenderCommand.h"

namespace BfUI {

enum class WidgetEvent : uint32_t
{
    None = 0,

    /* Button Events */
    ButtonPress = 1 << 0,
    ButtonRelease = 1 << 1,
    ButtonHover = 1 << 2,


    WindowHover = 1 << 2,
    WindowPress = 1 << 2,
    WindowRelease = 1 << 2,
};

enum class Edge
{
    None,
    Left,
    Right,
    Bottom,
    Top,
    CornerTopLeft,
    CornerTopRight,
    CornerBottomLeft,
    CornerBottomRight
};

class Widget

{
public:
public:
    Widget();
    virtual ~Widget() = default;


    virtual const DrawListData GetDrawList() const = 0;
    virtual void OnEvent(const Forge::Event& event) = 0;
    virtual const std::shared_ptr<Forge::Material>& GetMaterial() const
    {
        return m_DefaultMaterial;
    };
    virtual const Forge::BufferLayout& GetLayout() const
    {
        return m_WidgetBufferLayout;
    };

private:
    std::shared_ptr<Forge::Material> m_DefaultMaterial;
    Forge::BufferLayout m_WidgetBufferLayout;
};

}  // namespace BfUI


#endif
