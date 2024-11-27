//
// Created by toor on 2024-11-21.
//

#ifndef WIDGET_H
#define WIDGET_H


#include <cstdint>
#include <functional>

#include "Forge/Events/Event.h"
#include "Forge/Renderer/Material.h"
#include "Forge/Renderer/Buffer/Buffer.h"


namespace BfUI {

enum class WidgetEvents : uint32_t
{
    None = 0,

    /* Button Events */
    ButtonPress = 1 << 0,
    ButtonRelease = 1 << 1,
    ButtonHover = 1 << 2,

    /* WindowPanel Events */
    /*.......*/
    /* Other Events */
};

struct WidgetVertex
{
    glm::vec3 Position;
    glm::vec4 Color;
    glm::vec2 TexCoord;
    float TexIndex;
    float ElementType;
};

class Widget : public Forge::Event
{
public:
public:
    Widget();
    virtual ~Widget();


    virtual void OnEvent(const Event& event) = 0;
    virtual const std::vector<uint8_t>& GetVertices() const = 0;
    virtual const std::vector<uint32_t>& GetIndices() const = 0;


    const std::shared_ptr<Forge::Material>& GetMaterial();
    const Forge::BufferLayout& GetLayout();


private:
    std::shared_ptr<Forge::Material> m_WidgetMaterial;
    Forge::BufferLayout m_WidgetBufferLayout;
};

}  // namespace BfUI


#endif
