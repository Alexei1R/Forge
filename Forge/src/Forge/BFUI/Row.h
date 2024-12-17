
#ifndef ROW_H
#define ROW_H

#include <memory>
#include <vector>

#include "Widget.h"

namespace bf {
class Row : public Widget, public std::enable_shared_from_this<Row> {
protected:
    Row();

public:
    static std::shared_ptr<Row> Create(std::initializer_list<std::shared_ptr<Widget>> widgets);

    void AddChild(std::shared_ptr<Widget> child) override;
    const DrawListData GetDrawList() override;
    void OnEvent(const Forge::Event& event) override;

    vec2i GetPosition() const override;
    vec2i GetSize() const override;
    vec4i GetPadding() const override;

    std::shared_ptr<Widget> SetPosition(const vec2i& position) override;
    std::shared_ptr<Widget> SetSize(const vec2i& size) override;
    std::shared_ptr<Widget> SetPadding(const vec4i& padding) override;

    void SetParent(std::shared_ptr<Widget> parentWidget) override;

private:
    vec2i m_Size;
    vec2i m_Position;

    vec4i m_Padding;

    // WARN: Layout Usage Code
    std::vector<std::shared_ptr<Widget>> m_Children;
    std::shared_ptr<Widget> m_ParentWidget;
};
} // namespace bf
#endif
