
#ifndef TEXTVIEW_H
#define TEXTVIEW_H


#include "Forge/BFUI/Widget.h"
#include <string>

namespace bf {

class TextView : public Widget, public std::enable_shared_from_this<TextView>
{
protected:
    TextView(const std::string& text);
    const DrawListData GetDrawList() override;

public:
    static std::shared_ptr<TextView> Create(const std::string& text);
    vec2i GetPosition() const override;
    vec2i GetSize() const override;
    vec4i GetPadding() const override;

    std::shared_ptr<Widget> SetPosition(const vec2i& position) override;
    std::shared_ptr<Widget> SetSize(const vec2i& size) override;
    std::shared_ptr<Widget> SetPadding(const vec4i& padding) override;

    std::shared_ptr<Widget> SetText(const std::string& text) override;

    // WARN: Is Not intended to be used by user
    void SetParent(std::shared_ptr<Widget> parentWidget) override;
    void AddChild(std::shared_ptr<Widget> child) override;
    void OnEvent(const Forge::Event& event) override;
    void Update();

private:
    std::string m_Text;
    vec2i m_Size;
    vec2i m_Position;
    DrawListData m_DrawListData;

    vec4f m_ColorBackground;

    vec4i m_Padding;


    // WARN: Layout Usage Code
    std::vector<std::shared_ptr<Widget>> m_Children;
    std::shared_ptr<Widget> m_ParentWidget;
};


}  // namespace bf


#endif
