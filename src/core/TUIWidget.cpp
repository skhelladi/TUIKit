#include "tuikit/core/TUIWidget.h"
#include <ftxui/component/screen_interactive.hpp>

namespace TUIKIT {

TUIWidget::TUIWidget(ftxui::Component component) : component_(component) {}

void TUIWidget::requestRedraw() {
    if (auto* screen = ftxui::ScreenInteractive::Active()) {
        screen->PostEvent(ftxui::Event::Custom);
    }
}

} // namespace TUIKIT