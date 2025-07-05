#include "tuikit/widgets/TUIComboBox.h"
#include <ftxui/component/component.hpp>

namespace TUIKIT {

TUIComboBox::TUIComboBox(const std::vector<std::string>& options, int initial_selected, TUIWidget* parent)
    : TUIWidget(parent), options_(options), selected_index_(initial_selected), last_known_index_(initial_selected) {
    update_ftxui_dropdown_component();
}

void TUIComboBox::onSelect(OnSelectCallback cb) {
    on_select_ = cb;
    if (on_select_) {
        on_select_(selected_index_);
    }
}

void TUIComboBox::setOptions(const std::vector<std::string>& options) {
    options_ = options;
    selected_index_ = 0;
    last_known_index_ = 0;
    update_ftxui_dropdown_component();
    if (on_select_) {
        on_select_(selected_index_);
    }
}

int TUIComboBox::selectedIndex() const {
    return selected_index_;
}

std::string TUIComboBox::selectedText() const {
    if (selected_index_ >= 0 && static_cast<size_t>(selected_index_) < options_.size()) {
        return options_[selected_index_];
    }
    return "";
}

ftxui::Component TUIComboBox::get_ftxui_component() {
    return ftxui_dropdown_component_;
}

void TUIComboBox::setSelectedIndex(int index) {
    if (index >= 0 && static_cast<size_t>(index) < options_.size()) {
        selected_index_ = index;
        if (on_select_) {
            on_select_(selected_index_);
        }
    }
}

void TUIComboBox::update_ftxui_dropdown_component() {
    ftxui_dropdown_component_ = ftxui::Dropdown(&options_, &selected_index_);
    ftxui_dropdown_component_ = ftxui::Renderer(ftxui_dropdown_component_, [this] {
        if (last_known_index_ != selected_index_) {
            last_known_index_ = selected_index_;
            if (on_select_) {
                on_select_(selected_index_);
            }
        }
        return ftxui_dropdown_component_->Render();
    });
}

} // namespace TUIKIT