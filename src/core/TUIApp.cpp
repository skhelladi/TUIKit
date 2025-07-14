#include "tuikit/core/TUIApp.h"
#include "tuikit/core/TUIKLoader.h" // Include TUIKLoader
#include <iostream>

namespace TUIKIT {

using namespace ftxui;

TUIApp::TUIApp(const std::string& title)
    : title_(title), screen_(ftxui::ScreenInteractive::Fullscreen()), ui_loader_(std::make_unique<TUIKLoader>()) {
    std::cerr << "[DEBUG] TUIApp constructor: screen_ initialized." << std::endl << std::flush;
}

void TUIApp::set_theme(Theme theme) {
    TUIStyle::instance().setGlobalTheme(theme);
}

void TUIApp::show_status_bar(bool show) {
    show_status_bar_ = show;
}

void TUIApp::set_status_text(const std::string& status) {
    status_text_ = status;
}

void TUIApp::PostEvent(ftxui::Event event) {
    screen_.PostEvent(event);
}

void TUIApp::show_modal(ftxui::Component modal) {
    modal_ = modal;
    show_modal_ = true;
}

void TUIApp::close_modal() {
    show_modal_ = false;
    modal_ = nullptr;
}

std::shared_ptr<TUIWidget> TUIApp::loadUI(const std::string& filepath) {
    return ui_loader_->load(filepath);
}

std::shared_ptr<TUIWidget> TUIApp::getWidget(const std::string& name) {
    return ui_loader_->getWidget(name);
}

void TUIApp::show_message(const std::string& message, const std::string& title) {
    auto ok_button = ftxui::Button("OK", [this] { close_modal(); });
    auto dialog = ftxui::Renderer(ok_button, [=] {
        return ftxui::vbox({
            ftxui::text(title) | ftxui::bold,
            ftxui::separator(),
            ftxui::paragraph(message),
            ftxui::separator(),
            ok_button->Render() | ftxui::center
        }) | ftxui::border | ftxui::size(ftxui::WIDTH, ftxui::LESS_THAN, 50) | ftxui::center;
    });
    show_modal(dialog);
}

void TUIApp::show_error(const std::string& message, const std::string& title) {
    auto ok_button = ftxui::Button("OK", [this] { close_modal(); });
    auto dialog = ftxui::Renderer(ok_button, [=] {
        return ftxui::vbox({
            ftxui::text(title) | ftxui::bold | ftxui::color(ftxui::Color::Red),
            ftxui::separator(),
            ftxui::paragraph(message) | ftxui::color(ftxui::Color::Red),
            ftxui::separator(),
            ok_button->Render() | ftxui::center
        }) | ftxui::border | ftxui::size(ftxui::WIDTH, ftxui::LESS_THAN, 50) | ftxui::center;
    });
    show_modal(dialog);
}

void TUIApp::confirm(const std::string& message, std::function<void(bool)> callback, const std::string& title) {
    auto ok_button = ftxui::Button("OK", [this, callback] {
        close_modal();
        if (callback) callback(true);
    });
    auto cancel_button = ftxui::Button("Cancel", [this, callback] {
        close_modal();
        if (callback) callback(false);
    });
    auto buttons = ftxui::Container::Horizontal({ok_button, cancel_button});
    auto dialog = ftxui::Renderer(buttons, [=] {
        return ftxui::vbox({
            ftxui::text(title) | ftxui::bold,
            ftxui::separator(),
            ftxui::paragraph(message),
            ftxui::separator(),
            buttons->Render() | ftxui::center
        }) | ftxui::border | ftxui::size(ftxui::WIDTH, ftxui::LESS_THAN, 50) | ftxui::center;
    });
    show_modal(dialog);
}

int TUIApp::exec() {
    // std::cerr << "[TUIApp::exec] - Starting exec()" << std::endl << std::flush;

    if (pages_.empty() && !main_widget_) {
        // std::cerr << "[TUIApp::exec] - Error: No pages or main widget have been added to the application." << std::endl << std::flush;
        return 1;
    }

    ftxui::Component main_content_component;

    if (!pages_.empty()) {
        auto menu = ftxui::Menu(&page_names_, &active_page_);
        auto page_container = ftxui::Container::Tab(pages_, &active_page_);
        main_content_component = ftxui::Container::Horizontal({
            menu,
            page_container,
        });
    } else if (main_widget_) {
        main_content_component = main_widget_->get_ftxui_component();
    }

    auto final_interactive_root = ftxui::Renderer(main_content_component, [&] {
        auto& theme = TUIStyle::instance().currentTheme();
        ftxui::Element decorated_view = ftxui::vbox({
            ftxui::text(title_) | ftxui::bold | ftxui::center | ftxui::color(theme.primary),
            ftxui::separator(),
            main_content_component->Render() | ftxui::flex,
        });

        if (show_status_bar_) {
            decorated_view = ftxui::vbox({
                decorated_view | ftxui::flex,
                ftxui::separator(),
                ftxui::text(status_text_) | ftxui::color(theme.text),
            });
        }

        if (show_modal_ && modal_) {
            return ftxui::dbox({decorated_view, modal_->Render() | ftxui::clear_under | ftxui::center});
        }
        return decorated_view;
    });

    if (show_modal_ && modal_) {
        final_interactive_root = ftxui::Container::Vertical({final_interactive_root, ftxui::Maybe(modal_, &show_modal_)});
    }

    screen_.Loop(final_interactive_root);

    if (on_exit_) {
        on_exit_();
    }
    return 0;
}

void TUIApp::setMainWidget(std::shared_ptr<TUIWidget> widget) {
    std::cerr << "[TUIApp] Setting main widget" << std::endl;
    main_widget_ = widget;
}

void TUIApp::setOnExit(std::function<void()> callback) {
    on_exit_ = callback;
}

void TUIApp::request_redraw() {
    screen_.PostEvent(ftxui::Event::Custom);
}

void TUIApp::exit() {
    screen_.Exit();
}

void TUIApp::add_page(const std::string& name, ftxui::Component page) {
    page_names_.push_back(name);
    pages_.push_back(page);
}

void TUIApp::set_active_page(int index) {
    if (index >= 0 && static_cast<size_t>(index) < pages_.size()) {
        active_page_ = index;
    }
}

} // namespace TUIKIT