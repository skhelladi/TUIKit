#include "tuikit/widgets/TUIComboBox.h"
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/captured_mouse.hpp>
#include <ftxui/component/component_base.hpp>

namespace TUIKIT {

TUIComboBox::TUIComboBox(const std::vector<std::string>& options, int initial_selected, TUIWidget* parent)
    : TUIWidget(parent), options_(options), selected_index_(initial_selected) {
    update_ftxui_dropdown_component();
}

void TUIComboBox::onSelect(OnSelectCallback cb) {
    on_select_ = cb;
}

void TUIComboBox::setOptions(const std::vector<std::string>& options) {
    options_ = options;
    selected_index_ = 0;
    update_ftxui_dropdown_component();
}

int TUIComboBox::selectedIndex() const {
    return selected_index_;
}

std::string TUIComboBox::selectedText() const {
    if (selected_index_ >= 0 && selected_index_ < options_.size()) {
        return options_[selected_index_];
    }
    return "";
}

ftxui::Component TUIComboBox::get_ftxui_component() {
    return ftxui_dropdown_component_;
}

void TUIComboBox::setSelectedIndex(int index) {
    if (index >= 0 && index < options_.size()) {
        selected_index_ = index;
        // Force update of the FTXUI component to reflect the new selection
        update_ftxui_dropdown_component();
    }
}

void TUIComboBox::update_ftxui_dropdown_component() {
    int prev_selected_index = selected_index_;
    ftxui_dropdown_component_ = ftxui::Dropdown(&options_, &selected_index_);
    ftxui_dropdown_component_ = ftxui::Renderer(ftxui_dropdown_component_, [this, prev_selected_index] {
        if (prev_selected_index != selected_index_) {
            if (on_select_) {
                on_select_(selected_index_);
            }
        }
        return ftxui_dropdown_component_->Render();
    });
}

} // namespace TUIKIT