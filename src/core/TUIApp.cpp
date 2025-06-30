
#include "tuikit/core/TUIApp.h"
#include "tuikit/core/TUIWidget.h"
#include "tuikit/core/TUIStyle.h"
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <iostream>

namespace TUIKIT {

using namespace ftxui;

TUIApp::TUIApp(const std::string& title)
    : title_(title), screen_(ftxui::ScreenInteractive::Fullscreen()) {}

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
    std::cerr << "[TUIApp::exec] - Starting exec()" << std::endl;

    if (pages_.empty() && !main_widget_) {
        std::cerr << "[TUIApp::exec] - Error: No pages or main widget have been added to the application." << std::endl;
        return 1;
    }

    // Determine the base component to display
    ftxui::Component base_component;
    if (!pages_.empty()) {
        std::cerr << "[TUIApp::exec] - Using pages structure." << std::endl;
        auto menu = ftxui::Menu(&page_names_, &active_page_);
        auto page_container = ftxui::Container::Tab(pages_, &active_page_);

        base_component = ftxui::Container::Horizontal({
            menu,
            page_container,
        });
    } else if (main_widget_) {
        std::cerr << "[TUIApp::exec] - Using main_widget_." << std::endl;
        base_component = main_widget_->get_ftxui_component();
    }

    std::cerr << "[TUIApp::exec] - Creating main_layout_component_ renderer." << std::endl;
    main_layout_component_ = ftxui::Renderer(base_component, [this, base_component] {
        auto& theme = TUIStyle::instance().currentTheme();

        ftxui::Element rendered_base = base_component->Render();

        ftxui::Element decorated_view = ftxui::vbox({
            ftxui::text(title_) | ftxui::bold | ftxui::center | ftxui::color(theme.primary),
            ftxui::separator(),
            rendered_base | ftxui::flex,
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

    std::cerr << "[TUIApp::exec] - Creating final_interactive_root." << std::endl;
    std::vector<ftxui::Component> children;
    children.push_back(main_layout_component_);
    if (show_modal_ && modal_) {
        children.push_back(ftxui::Maybe(modal_, &show_modal_));
    }
    auto final_interactive_root = ftxui::Container::Vertical(children);

    std::cerr << "[TUIApp::exec] - Creating event_dispatcher." << std::endl;
    auto event_dispatcher = ftxui::CatchEvent(final_interactive_root, [&](ftxui::Event event) {
        if (show_modal_) {
            return modal_->OnEvent(event);
        } else {
            if (!pages_.empty() && active_page_ >= 0 && active_page_ < pages_.size()) {
                ftxui::Component active_page_component = pages_[active_page_];
                if (active_page_component->OnEvent(event)) {
                    return true;
                }
            }
            return main_layout_component_->OnEvent(event);
        }
    });

    std::cerr << "[TUIApp::exec] - Calling screen_.Loop()." << std::endl;
    screen_.Loop(event_dispatcher);
    std::cerr << "[TUIApp::exec] - screen_.Loop() returned." << std::endl;

    if (on_exit_) {
        std::cerr << "[TUIApp::exec] - Calling on_exit_ callback." << std::endl;
        on_exit_();
    }
    std::cerr << "[TUIApp::exec] - Exiting exec()." << std::endl;
    return 0;
}

void TUIApp::setMainWidget(std::shared_ptr<TUIWidget> widget) {
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
    if (index >= 0 && index < pages_.size()) {
        active_page_ = index;
    }
}

} // namespace TUIKIT
