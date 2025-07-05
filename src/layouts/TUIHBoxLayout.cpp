#include "tuikit/layouts/TUIHBoxLayout.h"
#include <ftxui/component/component.hpp>

namespace TUIKIT {

TUIHBoxLayout::TUIHBoxLayout(TUIWidget* /*parent*/)
    : TUILayout(ftxui::Container::Horizontal({})) {}

void TUIHBoxLayout::addWidget(std::shared_ptr<TUIWidget> widget) {
    widgets_.push_back(widget);
    component_->Add(widget->get_ftxui_component());
}

} // namespace TUIKIT