#include "tuikit/widgets/TUILabel.h"
#include "tuikit/core/TUIStyle.h"
#include <ftxui/component/component.hpp>

namespace TUIKIT {

TUILabel::TUILabel(const std::string& text, TUIWidget* /*parent*/)
    : TUIWidget(ftxui::Renderer([this] {
        auto& theme = TUIStyle::instance().currentTheme();
        return ftxui::text(text_) | ftxui::color(theme.text);
    })), text_(text) {
}

void TUILabel::setText(const std::string& text) {
    text_ = text;
}

std::string TUILabel::text() const {
    return text_;
}

} // namespace TUIKIT