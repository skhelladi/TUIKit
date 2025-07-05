#include "tuikit/widgets/TUIProgressBar.h"
#include <ftxui/dom/elements.hpp>
#include <algorithm> // For std::clamp

namespace TUIKIT {

TUIProgressBar::TUIProgressBar(float initial_value, const std::string& label, TUIWidget* /*parent*/)
    : TUIWidget(ftxui::Renderer([this] {
        auto progress_text = std::to_string(static_cast<int>(value_ * 100)) + "%";
        if (!label_.empty()) {
            progress_text = label_ + " " + progress_text;
        }
        return ftxui::vbox({
            ftxui::text(progress_text),
            ftxui::gauge(value_)
        });
    })), value_(initial_value), label_(label) {
}

void TUIProgressBar::setValue(float value) {
    value_ = std::clamp(value, 0.0f, 1.0f);
}

float TUIProgressBar::value() const {
    return value_;
}

void TUIProgressBar::setLabel(const std::string& label) {
    label_ = label;
}

} // namespace TUIKIT