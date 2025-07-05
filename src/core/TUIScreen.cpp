#include "tuikit/core/TUIScreen.h"

namespace TUIKIT {

TUIScreen::TUIScreen(TUIWidget* /*parent*/) : TUIWidget(ftxui::Component()) {}

void TUIScreen::setLayout(std::shared_ptr<TUIWidget> layout_widget) {
    layout_widget_ = layout_widget;
    if (layout_widget_) {
        component_ = layout_widget_->get_ftxui_component();
    }
}

} // namespace TUIKIT