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
    Widget();
    virtual ~Widget() = default;


    virtual const DrawListData GetDrawList() = 0;
    virtual void OnEvent(const Forge::Event& event) = 0;


    virtual const std::shared_ptr<Forge::Material>& GetMaterial() const;
    virtual const Forge::BufferLayout& GetLayout() const;

    virtual void AddChild(std::shared_ptr<Widget> child) = 0;


    virtual void SetParent(std::shared_ptr<Widget> parentWidget) = 0;


    // Layout properties
    virtual void SetPosition(const vec2i& position) {};
    virtual void SetSize(const vec2i& size) {};
    virtual vec2i GetPosition() const
    {
        return vec2i(0);
    };
    virtual vec2i GetSize() const
    {
        return vec2i(0);
    };


protected:
private:
    std::shared_ptr<Forge::Material> m_DefaultMaterial;
    Forge::BufferLayout m_WidgetBufferLayout;
};

}  // namespace BfUI


#endif
