#include "tuikit/widgets/TUIButton.h"
#include "tuikit/core/TUIStyle.h"
#include <ftxui/component/component.hpp>

namespace TUIKIT {

TUIButton::TUIButton(const std::string& text, TUIWidget* parent)
    : TUIWidget(parent), original_text_(text), text_(text) {
    ftxui_button_component_ = ftxui::Button(&text_, [this] { 
        if (on_clicked_) on_clicked_(); 
    });
}

ftxui::Component TUIButton::get_ftxui_component() {
    return ftxui::Renderer(ftxui_button_component_, [this] {
        auto& theme = TUIStyle::instance().currentTheme();
        return ftxui_button_component_->Render() | ftxui::color(theme.text) | ftxui::bgcolor(theme.primary);
    });
}

void TUIButton::onClick(OnClickedCallback callback) {
    on_clicked_ = callback;
}

void TUIButton::setIcon(const std::string& icon) {
    icon_ = icon;
    text_ = icon_ + " " + original_text_;
    ftxui_button_component_ = ftxui::Button(&text_, [this] { 
        if (on_clicked_) on_clicked_(); 
    });
}

const std::string& TUIButton::icon() const {
    return icon_;
}

} // namespace TUIKIT