#include "tuikit/widgets/TUIStatusBar.h"
#include "tuikit/widgets/TUILabel.h"
#include <ftxui/dom/elements.hpp>

namespace TUIKIT {

TUIStatusBar::TUIStatusBar(const std::string& message, TUIWidget* parent)
    : TUIWidget(parent) {
    message_label_ = new TUILabel(message);
    ftxui_status_bar_component_ = ftxui::Renderer(message_label_->get_ftxui_component(), [this] {
        return message_label_->get_ftxui_component()->Render() | ftxui::border;
    });
}

ftxui::Component TUIStatusBar::get_ftxui_component() {
    return ftxui_status_bar_component_;
}

void TUIStatusBar::setMessage(const std::string& message) {
    message_label_->setText(message);
}

std::string TUIStatusBar::message() const {
    return message_label_->text();
}

} // namespace TUIKIT