#include "tuikit/widgets/TUIResizableSplit.h"
#include <ftxui/component/component.hpp>

namespace TUIKIT {

TUIResizableSplit::TUIResizableSplit(std::shared_ptr<TUIWidget> first_widget, std::shared_ptr<TUIWidget> second_widget, Orientation orientation, TUIWidget* parent)
    : TUIWidget(parent), first_widget_(first_widget), second_widget_(second_widget), orientation_(orientation) {
    if (orientation == Horizontal) {
        ftxui_split_component_ = ftxui::ResizableSplitLeft(first_widget_->get_ftxui_component(), second_widget_->get_ftxui_component(), &main_size_);
    } else {
        ftxui_split_component_ = ftxui::ResizableSplitTop(first_widget_->get_ftxui_component(), second_widget_->get_ftxui_component(), &main_size_);
    }
}

ftxui::Component TUIResizableSplit::get_ftxui_component() {
    return ftxui_split_component_;
}

void TUIResizableSplit::setMinimumSizes(int min_first, int min_second) {
    min_first_ = min_first;
    min_second_ = min_second;
    // ftxui doesn't directly support setting minimum sizes on the resizable split component after creation.
    // This would need to be handled at a higher level, or by recreating the component.
}

} // namespace TUIKIT