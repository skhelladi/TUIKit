
#include "tuikit/core/TUIScreen.h"

namespace TUIKIT {

TUIScreen::TUIScreen(TUIWidget* parent) : TUIWidget(parent) {}

void TUIScreen::setLayout(std::shared_ptr<TUIWidget> layout_widget) {
    layout_widget_ = layout_widget;
}

ftxui::Component TUIScreen::get_ftxui_component() {
    if (layout_widget_) {
        return layout_widget_->get_ftxui_component();
    }
    return ftxui::Component(); // Return an empty component if no layout is set
}

} // namespace TUIKIT
