#include "tuikit/widgets/TUIResizableSplit.h"
#include <ftxui/component/component.hpp>
#include <ftxui/component/event.hpp>
#include <iostream> // For debug output

namespace TUIKIT {

TUIResizableSplit::TUIResizableSplit(std::shared_ptr<TUIWidget> first_widget, std::shared_ptr<TUIWidget> second_widget, Orientation orientation, TUIWidget* /*parent*/)
    : TUIWidget(ftxui::Component()), first_widget_(first_widget), second_widget_(second_widget), orientation_(orientation) {
    std::cerr << "TUIResizableSplit: Constructor called." << std::endl; // Debug output
    if (orientation == Horizontal) {
        component_ = ftxui::ResizableSplitLeft(first_widget_->get_ftxui_component(), second_widget_->get_ftxui_component(), &main_size_);
    } else {
        component_ = ftxui::ResizableSplitTop(first_widget_->get_ftxui_component(), second_widget_->get_ftxui_component(), &main_size_);
    }
}

void TUIResizableSplit::setMinimumSizes(int min_first, int min_second) {
    min_first_ = min_first;
    min_second_ = min_second;
}

} // namespace TUIKIT