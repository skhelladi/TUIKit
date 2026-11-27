#include "tuikit/widgets/TUIToolbar.h"
#include <ftxui/dom/elements.hpp>

namespace TUIKIT {

TUIToolbar::TUIToolbar(TUIWidget* /*parent*/)
    : TUIWidget(ftxui::Component()) {
    toolbar_layout_ = std::make_shared<TUIHBoxLayout>();
    component_ = toolbar_layout_->get_ftxui_component();
}

std::shared_ptr<TUIButton> TUIToolbar::addButton(const std::string& text, TUIButton::OnClickedCallback on_clicked, const std::string& icon) {
    if (item_count_ > 0) {
        // Breathing room between toolbar items.
        toolbar_layout_->addWidget(
            std::make_shared<TUILayout>(ftxui::Renderer([] {
                return ftxui::text("  ");
            })));
    }
    auto button = std::make_shared<TUIButton>(text);
    if (on_clicked) {
        button->onClick(on_clicked);
    }
    if (!icon.empty()) {
        button->setIcon(icon);
    }
    toolbar_layout_->addWidget(button);
    ++item_count_;
    return button;
}

} // namespace TUIKIT