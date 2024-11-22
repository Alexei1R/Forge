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

enum class BfUIEvents : uint32_t
{
    None = 0,

    /* Button Events */
    ButtonPress = 1 << 0,
    ButtonRelease = 1 << 1,
    ButtonHold = 1 << 2,
    ButtonHover = 1 << 3,

    /* WindowPanel Events */
    /*.......*/
    /* Other Events */
};

// NOTE: Pure abstract class , that will reprezent a GUI elemrnt
class Widget : public Forge::Event
{
public:
public:
    Widget() = default;
    virtual ~Widget() = default;


    virtual void OnEvent(const Event& event) = 0;
    virtual const std::vector<uint8_t>& GetVertices() const = 0;
    virtual const std::vector<uint32_t>& GetIndices() const = 0;
    virtual const std::shared_ptr<Forge::Material>& GetMaterial() const = 0;
    virtual const Forge::BufferLayout& GetLayout() const = 0;


private:
};

}  // namespace BfUI


#endif
