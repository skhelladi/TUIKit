#include "tuikit/widgets/TUITextField.h"
#include "tuikit/core/TUIStyle.h"
#include <ftxui/component/component.hpp>
#include <ftxui/component/event.hpp>

namespace TUIKIT {

TUITextField::TUITextField(const std::string& placeholder, TUIWidget* /*parent*/)
    : value_("") {
    input_ftxui_component_ = ftxui::Input(&value_, placeholder, ftxui::InputOption{ .on_change = [this] { if (on_change_) { on_change_(value_); } }, .on_enter = [this] { if (on_enter_) { on_enter_(); } } });
    component_ = TUIStyle::decorateField(input_ftxui_component_);
}

void TUITextField::onChange(OnChangeCallback cb) {
    on_change_ = cb;
}

void TUITextField::onEnter(OnEnterCallback cb) {
    on_enter_ = cb;
}

std::string TUITextField::text() const {
    return value_;
}

void TUITextField::setText(const std::string& text) {
    value_ = text;
    requestRedraw();
}

void TUITextField::setReadOnly(bool read_only) {
    read_only_ = read_only;
    if (read_only_) {
        component_ = TUIStyle::decorateField(
            ftxui::CatchEvent(input_ftxui_component_,
                              [](ftxui::Event /*event*/) { return true; }));
    } else {
        component_ = TUIStyle::decorateField(input_ftxui_component_);
    }
}

bool TUITextField::isReadOnly() const {
    return read_only_;
}

} // namespace TUIKIT

