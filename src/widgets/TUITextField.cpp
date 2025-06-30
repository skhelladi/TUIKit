#include "tuikit/widgets/TUITextField.h"
#include <ftxui/component/component.hpp>

namespace TUIKIT {

TUITextField::TUITextField(const std::string& placeholder, TUIWidget* parent)
    : TUIWidget(parent) {
}

void TUITextField::onChange(OnChangeCallback cb) {
    on_change_ = cb;
}

std::string TUITextField::text() const {
    return value_;
}

ftxui::Component TUITextField::get_ftxui_component() {
    ftxui::InputOption opt;
    opt.on_change = [this] {
        if (on_change_) {
            on_change_(value_);
        }
    };
    return ftxui::Input(&value_, "", opt);
}

} // namespace TUIKIT