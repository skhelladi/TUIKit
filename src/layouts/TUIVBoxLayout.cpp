#include "tuikit/layouts/TUIVBoxLayout.h"
#include <ftxui/component/component.hpp>

namespace TUIKIT {

TUIVBoxLayout::TUIVBoxLayout(TUIWidget* parent) : TUILayout(parent) {}

void TUIVBoxLayout::addWidget(std::shared_ptr<TUIWidget> widget) {
    widgets_.push_back(widget);
}

ftxui::Component TUIVBoxLayout::get_ftxui_component() {
    ftxui::Components components;
    for (auto& widget : widgets_) {
        components.push_back(widget->get_ftxui_component());
    }
    return ftxui::Container::Vertical(components);
}

} // namespace TUIKIT