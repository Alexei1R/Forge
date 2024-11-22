//
// Created by toor on 2024-11-21.
//

#ifndef BUTTON_H
#define BUTTON_H

#include "Forge/BFUI/UIPrimitives/Panel.h"
#include "Forge/BFUI/Widget.h"
#include "Forge/Events/Event.h"
#include "Forge/Events/ImplEvent.h"
#include "Forge/Events/KeyCodes.h"
#include "glm/fwd.hpp"
#include <cstdint>
#include <string>
namespace BfUI {

class Button : public Widget
{
public:
    Button(glm::vec2 position, glm::vec2 size, const char* label);
    ~Button();


    void SubscribeEvents(std::function<void(BfUI::BfUIEvents events, Button& button)> callback);

    const std::vector<uint8_t>& GetVertices() const override;
    const std::vector<uint32_t>& GetIndices() const override;

    const std::shared_ptr<Forge::Material>& GetMaterial() const override;
    const Forge::BufferLayout& GetLayout() const override;


    // NOTE: Using Fluent Interface Pattern  Patterrn Set UI Properties
    /*Button& SetText(const std::string& text);*/
    /*Button& SetTextColor(glm::vec4 color);*/
    Button& SetBackGroundColor(glm::vec4 color);
    /*Button& SetBackGroundColorHover(glm::vec4 color);*/
    /*Button& SetBackGroundColorActive(glm::vec4 color);*/
    /**/
    /*Button& SetBorderColor(glm::vec4 color);*/
    /*Button& SetBorderSize(uint32_t size);*/

    bool InInBounds(glm::vec2 point);
    // Implement missing pure virtual methods from Event
    virtual Forge::EventType GetType() const override {}

    virtual Forge::Action GetAction() const override {}

    virtual std::string ToString() const override
    {
        return "Button Event";
    }
    void OnEvent(const Event& event) override
    {
        if (event.GetType() == Forge::EventType::Key)
        {
            Forge::KeyEvent keyEv = static_cast<const Forge::KeyEvent&>(event);
            if (keyEv.GetAction() == Forge::Action::MousePress)
            {
                auto [x, y] = Forge::Mouse::GetMousePosition();
                auto [width, height] = Forge::ApplicationStats::GetApplicationSize();
                if (InInBounds(glm::vec2(x, height - y)))
                {
                    SetBackGroundColor(glm::vec4(1.0, 0.0, 1.0, 1.0));
                }
                else
                {
                    SetBackGroundColor(glm::vec4(0.0, 1.0, 0.0, 1.0));
                }
            }
        }
    }

private:
    void UpdateDrawData();

private:
    Panel m_ButtonBody;
    std::shared_ptr<Forge::Material> m_ButtonMaterial;

    Forge::BufferLayout m_ButtonBufferLayout;

    std::vector<uint32_t> m_Indices;
    std::vector<uint8_t> m_VerticesBytes;
};


}  // namespace BfUI


#endif
