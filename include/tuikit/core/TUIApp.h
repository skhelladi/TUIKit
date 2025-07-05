#pragma once
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <functional>
#include <memory>
#include <string>
#include "tuikit/core/TUIStyle.h"
#include "tuikit/core/TUIKLoader.h"

namespace TUIKIT {

// Forward declaration to fix template argument errors
class TUIKLoader;

class TUIApp {
public:
    TUIApp(const std::string& title = "TUI App");
    int exec();
    void run() { exec(); }
    void setMainWidget(std::shared_ptr<class TUIWidget> widget);
    void setOnExit(std::function<void()> callback);
    void request_redraw();
    void exit();

    // Compatibilité API archives/example_usage.cpp
    void set_theme(Theme theme);
    void show_status_bar(bool show);
    void set_status_text(const std::string& status);
    void confirm(const std::string& message, std::function<void(bool)> callback, const std::string& title = "Confirmation");
    void show_message(const std::string& message, const std::string& title = "Information");
    void show_error(const std::string& message, const std::string& title = "Error");
    void PostEvent(ftxui::Event event); // Changed to non-const reference

    void show_modal(ftxui::Component modal);
    void close_modal();

    // UI Loading
    std::shared_ptr<class TUIWidget> loadUI(const std::string& filepath);
    std::shared_ptr<class TUIWidget> getWidget(const std::string& name);

// Ajout gestion de pages/menu
    void add_page(const std::string& name, ftxui::Component page);
    void set_active_page(int index);

private:
    std::string title_;
    std::shared_ptr<class TUIWidget> main_widget_ = nullptr;
    std::function<void()> on_exit_;
    ftxui::ScreenInteractive screen_;
    std::unique_ptr<TUIKLoader> ui_loader_; // Instance of TUIKLoader

    // Pour gestion menu/pages
    std::vector<std::string> page_names_;
    std::vector<ftxui::Component> pages_;
    int active_page_ = 0;

    ftxui::Component main_layout_component_ = nullptr;
    // For modal management
    ftxui::Component modal_ = nullptr;
    bool show_modal_ = false;

    // For status bar
    bool show_status_bar_ = false;
    std::string status_text_ = "Ready";

};

}
