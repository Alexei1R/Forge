//
// Created by toor on 2024-11-27.
//


#include "Widget.h"


namespace BfUI {

Widget::Widget() {}
Widget::~Widget() {}

const std::shared_ptr<Forge::Material>& Widget::GetMaterial()
{
    return m_WidgetMaterial;
};

const Forge::BufferLayout& Widget::GetLayout()
{
    return m_WidgetBufferLayout;
};

}  // namespace BfUI
