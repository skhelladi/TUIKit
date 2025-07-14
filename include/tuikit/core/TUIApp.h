#pragma once

#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>
#include <string>
#include <memory>
#include <functional>
#include <vector>

#include "tuikit/core/TUIWidget.h"
#include "tuikit/core/TUIStyle.h"

namespace TUIKIT {

class TUIKLoader; // Forward declaration

class TUIApp {
public:
    TUIApp(const std::string& title);
    int exec();
    void setMainWidget(std::shared_ptr<TUIWidget> widget);
    void setOnExit(std::function<void()> callback);
    void request_redraw();
    void exit();

    void add_page(const std::string& name, ftxui::Component page);
    void set_active_page(int index);

    void set_theme(Theme theme);
    void show_status_bar(bool show);
    void set_status_text(const std::string& status);

    void PostEvent(ftxui::Event event);

    // Modal dialogs
    void show_modal(ftxui::Component modal);
    void close_modal();
    void show_message(const std::string& message, const std::string& title = "Message");
    void show_error(const std::string& message, const std::string& title = "Error");
    void confirm(const std::string& message, std::function<void(bool)> callback, const std::string& title = "Confirm");

    // UI Loader
    std::shared_ptr<TUIWidget> loadUI(const std::string& filepath);
    std::shared_ptr<TUIWidget> getWidget(const std::string& name);

private:
    std::string title_;
    ftxui::ScreenInteractive screen_;
    std::shared_ptr<TUIWidget> main_widget_;
    ftxui::Component main_layout_component_;
    std::function<void()> on_exit_;

    std::vector<std::string> page_names_;
    std::vector<ftxui::Component> pages_;
    int active_page_ = 0;

    bool show_status_bar_ = false;
    std::string status_text_;

    ftxui::Component modal_;
    bool show_modal_ = false;

    std::unique_ptr<TUIKLoader> ui_loader_;
};

}
