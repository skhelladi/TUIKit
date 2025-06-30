#include "tuikit/layouts/TUIHBoxLayout.h"
#include <ftxui/component/component.hpp>

namespace TUIKIT {

TUIHBoxLayout::TUIHBoxLayout(TUIWidget* parent) : TUILayout(parent) {}

void TUIHBoxLayout::addWidget(std::shared_ptr<TUIWidget> widget) {
    widgets_.push_back(widget);
}

ftxui::Component TUIHBoxLayout::get_ftxui_component() {
    ftxui::Components components;
    for (auto& widget : widgets_) {
        components.push_back(widget->get_ftxui_component());
    }
    return ftxui::Container::Horizontal(components);
}

} // namespace TUIKIT