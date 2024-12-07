//
// Created by toor on 2024-12-03.
//

#ifndef COLUMN_H
#define COLUMN_H


#include "Forge/Core/Log/Log.h"
#include "Widget.h"
#include <vector>
#include <memory>

namespace BfUI {

class Column : public Widget, public std::enable_shared_from_this<Column>
{
protected:
    Column();

public:
    static std::shared_ptr<Column> Create(std::initializer_list<std::shared_ptr<Widget>> widgets);

    void AddChild(std::shared_ptr<Widget> child) override;
    const DrawListData GetDrawList() override;
    void OnEvent(const Forge::Event& event) override;


    vec2i GetPosition() const override;
    vec2i GetSize() const override;
    void SetPosition(const vec2i& position) override;
    void SetSize(const vec2i& size) override;

    void SetParent(std::shared_ptr<Widget> parentWidget) override;

private:
    std::shared_ptr<Widget> m_ParentWidget;
    vec2i m_Position;
    vec2i m_Size;
    std::vector<std::shared_ptr<Widget>> m_Children;
};


}  // namespace BfUI
#endif
