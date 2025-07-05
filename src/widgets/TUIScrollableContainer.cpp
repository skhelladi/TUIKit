#include "tuikit/widgets/TUIScrollableContainer.h"
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/component/event.hpp>
#include <iostream> // For debug output

namespace TUIKIT {

TUIScrollableContainer::TUIScrollableContainer(std::shared_ptr<TUIWidget> content, int height, TUIWidget* parent)
    : TUIWidget(parent), content_(content), height_(height) {
    ftxui_scrollable_component_ = content_->get_ftxui_component() | ftxui::vscroll_indicator | ftxui::frame | ftxui::size(ftxui::HEIGHT, ftxui::EQUAL, height_);
}

ftxui::Component TUIScrollableContainer::get_ftxui_component() {
    return ftxui_scrollable_component_;
}

} // namespace TUIKIT