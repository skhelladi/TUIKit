#include "tuikit/widgets/TUIToolbar.h"

namespace TUIKIT {

TUIToolbar::TUIToolbar(TUIWidget* /*parent*/)
    : TUIWidget(ftxui::Component()) {
    toolbar_layout_ = std::make_shared<TUIHBoxLayout>();
    component_ = toolbar_layout_->get_ftxui_component();
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

} // namespace TUIKIT