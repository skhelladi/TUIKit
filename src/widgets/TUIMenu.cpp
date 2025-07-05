#include "tuikit/widgets/TUIMenu.h"
#include <ftxui/component/component.hpp>

namespace TUIKIT {

TUIMenu::TUIMenu(const std::vector<std::string>& options, TUIWidget* /*parent*/)
    : TUIWidget(ftxui::Menu(&options_, &selected_index_)), options_(options) {
    update_ftxui_menu_component();
}

void TUIMenu::setOptions(const std::vector<std::string>& options) {
    options_ = options;
    update_ftxui_menu_component();
}

void TUIMenu::setIcons(const std::vector<std::string>& icons) {
    icons_ = icons;
    update_ftxui_menu_component();
}

void TUIMenu::onSelect(OnSelectCallback callback) {
    on_select_callback_ = callback;
}

int TUIMenu::selectedIndex() const {
    return selected_index_;
}

std::string TUIMenu::selectedText() const {
    if (selected_index_ >= 0 && static_cast<size_t>(selected_index_) < options_.size()) {
        return options_[selected_index_];
    }
    return "";
}

void TUIMenu::setSelectedIndex(int index) {
    if (index >= 0 && static_cast<size_t>(index) < options_.size()) {
        selected_index_ = index;
        // Force update of the FTXUI component to reflect the new selection
        update_ftxui_menu_component();
    }
}

void TUIMenu::update_ftxui_menu_component() {
    ftxui::MenuOption option;
    option.on_change = [this] {
        if (on_select_callback_) {
            on_select_callback_(selected_index_);
        }
    };

    if (!icons_.empty()) {
        options_with_icons_cache_.clear();
        for (size_t i = 0; i < options_.size(); ++i) {
            if (i < icons_.size()) {
                options_with_icons_cache_.push_back(icons_[i] + " " + options_[i]);
            } else {
                options_with_icons_cache_.push_back(options_[i]);
            }
        }
        component_ = ftxui::Menu(&options_with_icons_cache_, &selected_index_, option);
    } else {
        component_ = ftxui::Menu(&options_, &selected_index_, option);
    }
}

} // namespace TUIKIT