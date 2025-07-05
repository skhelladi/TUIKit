#include "tuikit/widgets/TUIButton.h"
#include "tuikit/core/TUIStyle.h"
#include <ftxui/component/component.hpp>

namespace TUIKIT {

TUIButton::TUIButton(const std::string& text, TUIWidget* /*parent*/)
    : original_text_(text), text_(text), TUIWidget(ftxui::Button(&text_, [this] { 
        if (on_clicked_) on_clicked_(); 
    })) {}

void TUIButton::onClick(OnClickedCallback callback) {
    on_clicked_ = callback;
}

void TUIButton::setIcon(const std::string& icon) {
    icon_ = icon;
    text_ = icon_ + " " + original_text_;
    // Recreate the FTXUI component with the new text
    component_ = ftxui::Button(&text_, [this] { 
        if (on_clicked_) on_clicked_(); 
    });
}

void TUIButton::setText(const std::string& text) {
    original_text_ = text;
    text_ = icon_ + " " + original_text_;
    // Recreate the FTXUI component with the new text
    component_ = ftxui::Button(&text_, [this] { 
        if (on_clicked_) on_clicked_(); 
    });
}

const std::string& TUIButton::icon() const {
    return icon_;
}

} // namespace TUIKIT