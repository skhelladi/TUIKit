#include "tuikit/widgets/TUITabWidget.h"
#include <ftxui/component/component.hpp>

namespace TUIKIT {

TUITabWidget::TUITabWidget(TabOrientation orientation, TUIWidget* /*parent*/) : TUIWidget(ftxui::Component()), orientation_(orientation) {
    update_ftxui_tab_component();
}

void TUITabWidget::addTab(const std::string& title, std::shared_ptr<TUIWidget> content_widget) {
    tab_titles_.push_back(title);
    tab_ftxui_components_.push_back(content_widget->get_ftxui_component());
    update_ftxui_tab_component();
}

void TUITabWidget::addTab(const std::string& title, std::shared_ptr<TUIWidget> content_widget, const std::string& icon) {
    tab_titles_.push_back(icon + " " + title);
    tab_ftxui_components_.push_back(content_widget->get_ftxui_component());
    update_ftxui_tab_component();
}

void TUITabWidget::setCurrentIndex(int index) {
    selected_tab_index_ = index;
}

int TUITabWidget::currentIndex() const {
    return selected_tab_index_;
}

void TUITabWidget::update_ftxui_tab_component() {
    auto toggle = ftxui::Toggle(&tab_titles_, &selected_tab_index_); 
    auto tab_container = ftxui::Container::Tab(tab_ftxui_components_, &selected_tab_index_);
    if (orientation_ == TabOrientation::Horizontal) {
        component_ = ftxui::Container::Vertical({
            toggle,
            tab_container
        });
    } else {
        component_ = ftxui::Container::Horizontal({
            toggle,
            tab_container
        });
    }
}

} // namespace TUIKIT