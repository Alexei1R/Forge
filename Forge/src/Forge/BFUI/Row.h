//
// Created by toor on 2024-12-03.
//

#ifndef ROW_H
#define ROW_H

#include "Widget.h"
#include <vector>
#include <memory>

namespace BfUI {
class Row : public Widget, public std::enable_shared_from_this<Row>
{
protected:
    Row();

public:
    static std::shared_ptr<Row> Create(std::initializer_list<std::shared_ptr<Widget>> widgets);


    void AddChild(std::shared_ptr<Widget> child) override;
    const DrawListData GetDrawList() override;
    void OnEvent(const Forge::Event& event) override;


    vec2i GetPosition() const override;
    vec2i GetSize() const override;
    void SetPosition(const vec2i& position) override;
    void SetSize(const vec2i& size) override;

    void SetParent(std::shared_ptr<Widget> parentWidget) override;

private:
    vec2i m_Position;
    vec2i m_Size;
    std::vector<std::shared_ptr<Widget>> m_Children;

    std::shared_ptr<Widget> m_ParentWidget;
};
}  // namespace BfUI
#endif
