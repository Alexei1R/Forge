
#ifndef WIDGET_H
#define WIDGET_H


#include <cstdint>
#include <vector>


#include "Forge/BFUI/DrawList.h"
#include "Forge/Core/Utils.h"
#include "Forge/Events/Event.h"
#include "Forge/Renderer/Buffer/Buffer.h"
#include "Forge/Renderer/Material.h"
#include "Forge/Renderer/RenderCommand.h"

namespace bf {

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
    None = 0,
    Left,
    Right,
    Top,
    Bottom,
    CornerTopLeft,
    CornerTopRight,
    CornerBottomLeft,
    CornerBottomRight
};

class Widget

{
protected:
    using EventCallback = std::function<void(WidgetEvent, Widget&)>;

public:
    Widget();
    virtual ~Widget() = default;


    virtual const DrawListData GetDrawList() = 0;
    virtual void OnEvent(const Forge::Event& event) = 0;


    virtual const std::shared_ptr<Forge::Material>& GetMaterial() const;
    virtual const Forge::BufferLayout& GetLayout() const;

    virtual void AddChild(std::shared_ptr<Widget> child) = 0;
    virtual void SetParent(std::shared_ptr<Widget> parentWidget) = 0;


    // Fluent Interface Setters
    virtual std::shared_ptr<Widget> SetPosition(const vec2i& position) = 0;
    virtual std::shared_ptr<Widget> SetSize(const vec2i& size) = 0;
    virtual std::shared_ptr<Widget> SetPadding(const vec4i& padding) = 0;

    // Getters
    virtual vec2i GetPosition() const = 0;
    virtual vec2i GetSize() const = 0;
    virtual vec4i GetPadding() const
    {
        return m_Padding;
    }
    virtual std::shared_ptr<Widget> SubscribeEvents(EventCallback callback)
    {
        F_ASSERT(false,
                 "This Widget does not support Events. Make sure that the class inherits and "
                 "overrides this function.");
    }

    virtual std::shared_ptr<Widget> SetText(const std::string& label)
    {
        F_ASSERT(false,
                 "This Widget does not support name setting. Make sure that the class inherits and "
                 "overrides this function.");
    }

private:
    vec4i m_Padding;
    std::shared_ptr<Forge::Material> m_DefaultMaterial;
    Forge::BufferLayout m_WidgetBufferLayout;
};

}  // namespace bf


#endif
