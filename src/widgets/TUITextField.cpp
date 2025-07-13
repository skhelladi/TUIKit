#include "tuikit/widgets/TUITextField.h"
#include <ftxui/component/component.hpp>

namespace TUIKIT {

TUITextField::TUITextField(const std::string& placeholder, TUIWidget* /*parent*/)
    : value_(""), TUIWidget(ftxui::Input(&value_, placeholder, ftxui::InputOption{ .on_change = [this] { if (on_change_) { on_change_(value_); } }, .on_enter = [this] { if (on_enter_) { on_enter_(); } } })) {}

void TUITextField::onChange(OnChangeCallback cb) {
    on_change_ = cb;
}

void TUITextField::onEnter(OnEnterCallback cb) {
    on_enter_ = cb;
}

std::string TUITextField::text() const {
    return value_;
}

} // namespace TUIKIT