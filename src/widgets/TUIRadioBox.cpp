#include "tuikit/widgets/TUIRadioBox.h"
#include <ftxui/component/component.hpp>

namespace TUIKIT {

TUIRadioBox::TUIRadioBox(const std::vector<std::string>& options, int initial_selected, TUIWidget* parent)
    : TUILayout(parent), options_(options), selected_index_(initial_selected) {
    update_ftxui_radiobox_component();
}

void TUIRadioBox::onSelect(OnSelectCallback cb) {
    on_select_ = cb;
}

void TUIRadioBox::setOptions(const std::vector<std::string>& options) {
    options_ = options;
    selected_index_ = 0;
    update_ftxui_radiobox_component();
}

int TUIRadioBox::selectedIndex() const {
    return selected_index_;
}

std::string TUIRadioBox::selectedText() const {
    if (selected_index_ >= 0 && static_cast<size_t>(selected_index_) < options_.size()) {
        return options_[selected_index_];
    }
    return "";
}

ftxui::Component TUIRadioBox::get_ftxui_component() {
    return ftxui_radiobox_component_;
}

void TUIRadioBox::setSelectedIndex(int index) {
    if (index >= 0 && static_cast<size_t>(index) < options_.size()) {
        selected_index_ = index;
        // Force update of the FTXUI component to reflect the new selection
        update_ftxui_radiobox_component();
    }
}

void TUIRadioBox::update_ftxui_radiobox_component() {
    ftxui::RadioboxOption opt;
    opt.on_change = [this] {
        if (on_select_) {
            on_select_(selected_index_);
        }
    };
    ftxui_radiobox_component_ = ftxui::Radiobox(&options_, &selected_index_, opt);
}

} // namespace TUIKIT