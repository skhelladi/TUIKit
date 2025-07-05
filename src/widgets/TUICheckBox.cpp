#include "tuikit/widgets/TUICheckBox.h"
#include <ftxui/component/component.hpp>

namespace TUIKIT {

TUICheckBox::TUICheckBox(const std::string& text, bool initial_checked, TUIWidget* /*parent*/)
    : TUIWidget(ftxui::Component()), checked_(initial_checked), text_(text) {
    ftxui::CheckboxOption opt;
    opt.on_change = [this] {
        if (on_change_) {
            on_change_(checked_);
        }
    };
    component_ = ftxui::Checkbox(&text_, &checked_, opt);
}

void TUICheckBox::onChange(OnChangeCallback cb) {
    on_change_ = cb;
}

bool TUICheckBox::checked() const {
    return checked_;
}

} // namespace TUIKIT