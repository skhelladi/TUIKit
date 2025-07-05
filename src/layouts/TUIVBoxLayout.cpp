#include "tuikit/layouts/TUIVBoxLayout.h"
#include <ftxui/component/component.hpp>

namespace TUIKIT {

TUIVBoxLayout::TUIVBoxLayout(TUIWidget* /*parent*/)
    : TUILayout(ftxui::Container::Vertical({})) {}

void TUIVBoxLayout::addWidget(std::shared_ptr<TUIWidget> widget) {
    widgets_.push_back(widget);
    component_->Add(widget->get_ftxui_component());
}

} // namespace TUIKIT