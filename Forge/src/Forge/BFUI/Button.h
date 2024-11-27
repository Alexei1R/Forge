//
// Created by toor on 2024-11-21.
//

#ifndef BUTTON_H
#define BUTTON_H

#include "Forge/BFUI/Widget.h"
#include "Forge/BFUI/WidgetStack.h"
#include "Forge/Core/Log/Log.h"
#include "Forge/Events/Event.h"
#include "Forge/Renderer/Material.h"
#include "Forge/Renderer/ShaderManager.h"

#include "Forge/BFUI/Theme.h"

#include <glm/glm.hpp>
#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace BfUI {

struct ButtonVertex
{
    glm::vec2 Position;
    glm::vec2 TexCoord;
};

class Button : public Widget, public std::enable_shared_from_this<Button>
{
public:
    using EventCallback = std::function<void(WidgetEvents, Button&)>;

    static std::shared_ptr<Button>
        Create(const glm::vec2& position, const glm::vec2& size, const std::string& label);

    ~Button();

    void SubscribeEvents(EventCallback callback);

    const std::vector<uint8_t>& GetVertices() const override;
    const std::vector<uint32_t>& GetIndices() const override;
    const std::shared_ptr<Forge::Material>& GetMaterial() const override;
    const Forge::BufferLayout& GetLayout() const override;


    void Move(const glm::vec2& position);

    Button& SetBackgroundColor(const glm::vec4& color);

    bool IsInBounds(const glm::vec2& point) const;

    Forge::EventType GetType() const override;
    Forge::Action GetAction() const override;
    std::string ToString() const override;

    void OnEvent(const Event& event) override;

protected:
    Button(const glm::vec2& position, const glm::vec2& size, const std::string& label);

private:
    void UpdateDrawData();

private:
    EventCallback m_EventCallback;

    glm::vec2 m_Position;
    glm::vec2 m_Size;

    glm::vec2 m_BottomLeft;
    glm::vec2 m_TopRight;
    glm::vec2 m_TopLeft;
    glm::vec2 m_BottomRight;

    bool m_IsPressed = false;
    bool m_IsHovered = false;

    glm::vec4 m_DefaultColor;
    glm::vec4 m_HoverColor;


    std::vector<uint32_t> m_Indices;
    std::vector<uint8_t> m_VerticesBytes;
};

}  // namespace BfUI

#endif  // BUTTON_H
