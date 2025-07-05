#include "tuikit/core/TUIKLoader.h" // Include TUIKLoader

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
    std::cerr << "[TUIApp::exec] - Starting exec()" << std::endl << std::flush;

    if (pages_.empty() && !main_widget_) {
        std::cerr << "[TUIApp::exec] - Error: No pages or main widget have been added to the application." << std::endl << std::flush;
        return 1;
    }

    // Determine the base component to display
    ftxui::Component base_component;
    if (!pages_.empty()) {
        std::cerr << "[TUIApp::exec] - Using pages structure." << std::endl << std::flush;
        auto menu = ftxui::Menu(&page_names_, &active_page_);
        auto page_container = ftxui::Container::Tab(pages_, &active_page_);

        base_component = ftxui::Container::Horizontal({
            menu,
            page_container,
        });
    } else if (main_widget_) {
        std::cerr << "[TUIApp::exec] - Using main_widget_." << std::endl << std::flush;
        if (!main_widget_->get_ftxui_component()) {
            std::cerr << "[TUIApp::exec] - Error: main_widget_ has a null FTXUI component." << std::endl << std::flush;
            return 1;
        }
        base_component = main_widget_->get_ftxui_component();
        std::cerr << "[TUIApp::exec] - base_component from main_widget_ obtained." << std::endl << std::flush;
    } else {
        std::cerr << "[TUIApp::exec] - Error: No base component to display." << std::endl << std::flush;
        return 1;
    }

    std::cerr << "[TUIApp::exec] - Creating main_layout_component_ renderer." << std::endl << std::flush;
    main_layout_component_ = ftxui::Renderer(base_component, [this, base_component] {
        std::cerr << "[TUIApp::exec] - Renderer lambda entered." << std::endl << std::flush;
        auto& theme = TUIStyle::instance().currentTheme();

        ftxui::Element rendered_base = base_component->Render();
        std::cerr << "[TUIApp::exec] - base_component->Render() called." << std::endl << std::flush;

        ftxui::Element decorated_view = ftxui::vbox({
            ftxui::text(title_) | ftxui::bold | ftxui::center | ftxui::color(theme.primary),
            ftxui::separator(),
            rendered_base | ftxui::flex,
        });
        std::cerr << "[TUIApp::exec] - decorated_view created." << std::endl << std::flush;

        if (show_status_bar_) {
            decorated_view = ftxui::vbox({
                decorated_view | ftxui::flex,
                ftxui::separator(),
                ftxui::text(status_text_) | ftxui::color(theme.text),
            });
            std::cerr << "[TUIApp::exec] - Status bar added." << std::endl << std::flush;
        }

        if (show_modal_ && modal_) {
            std::cerr << "[TUIApp::exec] - Modal detected, rendering modal." << std::endl << std::flush;
            return ftxui::dbox({decorated_view, modal_->Render() | ftxui::clear_under | ftxui::center});
        }
        std::cerr << "[TUIApp::exec] - Returning decorated_view." << std::endl << std::flush;
        return decorated_view;
    });

    std::cerr << "[TUIApp::exec] - Creating final_interactive_root." << std::endl << std::flush;
    std::vector<ftxui::Component> children;
    children.push_back(main_layout_component_);
    if (show_modal_ && modal_) {
        children.push_back(ftxui::Maybe(modal_, &show_modal_));
    }
    auto final_interactive_root = ftxui::Container::Vertical(children);
    std::cerr << "[TUIApp::exec] - final_interactive_root created." << std::endl << std::flush;

    std::cerr << "[TUIApp::exec] - About to call screen_.Loop()." << std::endl << std::flush;
    screen_.Loop(final_interactive_root);
    std::cerr << "[TUIApp::exec] - screen_.Loop() returned." << std::endl << std::flush;

    if (on_exit_) {
        std::cerr << "[TUIApp::exec] - Calling on_exit_ callback." << std::endl << std::flush;
        on_exit_();
    }
    std::cerr << "[TUIApp::exec] - Exiting exec()." << std::endl << std::flush;
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
    if (index >= 0 && static_cast<size_t>(index) < pages_.size()) {
        active_page_ = index;
    }
}

} // namespace TUIKIT