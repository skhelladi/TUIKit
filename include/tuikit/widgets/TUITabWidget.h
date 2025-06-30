#ifndef TUIKIT_TUITABWIDGET_H
#define TUIKIT_TUITABWIDGET_H

#include "tuikit/core/TUIWidget.h"
#include <string>
#include <vector>
#include <functional>
#include <ftxui/component/component.hpp>

namespace TUIKIT {

class TUITabWidget : public TUIWidget {
public:
    TUITabWidget(TUIWidget* parent = nullptr);
    ~TUITabWidget() override = default;

    void addTab(const std::string& title, std::shared_ptr<TUIWidget> content_widget);
    void addTab(const std::string& title, std::shared_ptr<TUIWidget> content_widget, const std::string& icon);
    ftxui::Component get_ftxui_component() override;

    void setCurrentIndex(int index);
    int currentIndex() const;

private:
    std::vector<std::string> tab_titles_;
    std::vector<std::string> tab_icons_;
    std::vector<ftxui::Component> tab_ftxui_components_;
    int selected_tab_index_ = 0;
    ftxui::Component ftxui_tab_component_;

    void update_ftxui_tab_component();
};

} // namespace TUIKIT

#endif // TUIKIT_TUITABWIDGET_H
