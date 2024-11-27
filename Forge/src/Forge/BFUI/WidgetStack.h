//
// Created by toor on 2024-11-22.
//

#ifndef WIDGETSTACK_H
#define WIDGETSTACK_H

#include "Forge/BFUI/Widget.h"
#include <vector>
#include <memory>

namespace BfUI {

class WidgetStack : public Widget
{
public:
    static void AddWidget(const std::shared_ptr<BfUI::Widget>& widget);

    static const std::vector<std::shared_ptr<Widget>>& GetAllWidgets();

    static auto begin();
    static auto end();

private:
    static std::vector<std::shared_ptr<Widget>> m_Widgets;
};

}  // namespace BfUI

#endif
