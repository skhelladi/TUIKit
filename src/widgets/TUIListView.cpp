#include "tuikit/widgets/TUIListView.h"
#include "ftxui/component/component.hpp"

namespace TUIKIT {

TUIListView::TUIListView(const std::vector<std::string>& items, TUIWidget* /*parent*/)
    : TUIWidget(nullptr), // Temporary initialization, will be replaced below
      items_(items) {
    update_ftxui_menu_component();
}

void TUIListView::onSelect(OnSelectCallback cb) {
    on_select_ = cb;
}

void TUIListView::onEnter(OnEnterCallback cb) {
    on_enter_ = cb;
}

int TUIListView::selectedIndex() const {
    return selected_index_;
}

void TUIListView::setSelectedIndex(int index) {
    if (index >= 0 && static_cast<size_t>(index) < items_.size()) {
        selected_index_ = index;
    }
}

std::string TUIListView::selectedText() const {
    if (const auto i = static_cast<size_t>(selected_index_); i >= 0 && i < items_.size()) {
        return items_[selected_index_];
    }
    return "";
}

void TUIListView::setItems(const std::vector<std::string>& items) {
    items_ = items;
    update_ftxui_menu_component();
}

void TUIListView::update_ftxui_menu_component() {
    auto menu_option = ftxui::MenuOption();
    menu_option.on_change = [this] { if (on_select_) { on_select_(selected_index_); } };
    menu_option.on_enter = [this] { if (on_enter_) { on_enter_(selected_index_); } };
    component_ = ftxui::Menu(&items_, &selected_index_, menu_option);
}

}