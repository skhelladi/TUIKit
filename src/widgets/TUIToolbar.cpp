#include "tuikit/widgets/TUIToolbar.h"

namespace TUIKIT {

TUIToolbar::TUIToolbar(TUIWidget* parent)
    : TUIWidget(parent) {
    toolbar_layout_ = std::make_shared<TUIHBoxLayout>();
}

std::shared_ptr<TUIButton> TUIToolbar::addButton(const std::string& text, TUIButton::OnClickedCallback on_clicked, const std::string& icon) {
    auto button = std::make_shared<TUIButton>(text);
    if (on_clicked) {
        button->onClick(on_clicked);
    }
    if (!icon.empty()) {
        button->setIcon(icon);
    }
    toolbar_layout_->addWidget(button);
    return button;
}

ftxui::Component TUIToolbar::get_ftxui_component() {
    return toolbar_layout_->get_ftxui_component();
}

} // namespace TUIKIT