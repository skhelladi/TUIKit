
#include "tuikit/widgets/TUISlider.h"
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>

namespace TUIKIT {

TUISlider::TUISlider(const std::string& label, float initial_value, float min_value, float max_value, float increment, TUIWidget* parent)
    : TUIWidget(parent), label_(label), value_(initial_value), min_value_(min_value), max_value_(max_value), increment_(increment) {
}

ftxui::Component TUISlider::get_ftxui_component() {
    ftxui::SliderOption<float> option;
    option.value = &value_;
    option.min = min_value_;
    option.max = max_value_;
    option.increment = increment_;
    option.on_change = [this] {
        if (on_change_callback_) {
            on_change_callback_(value_);
        }
    };

    auto slider_component = ftxui::Slider(option);

    return ftxui::Container::Vertical({
        ftxui::Renderer([this] { return ftxui::text(label_); }),
        slider_component | ftxui::border
    });
}

float TUISlider::value() const {
    return value_;
}

void TUISlider::setValue(float new_value) {
    value_ = new_value;
}

void TUISlider::onChange(OnChangeCallback callback) {
    on_change_callback_ = callback;
}

} // namespace TUIKIT
