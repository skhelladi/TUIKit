// =============================================================================
// TUIKIT FRAMEWORK IMPLEMENTATION WITH FTXUI
// =============================================================================

#include "tuikit_framework.h"

#include <ftxui/component/captured_mouse.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/color.hpp>

#include <algorithm> // For std::clamp
#include <iostream> // For debug outputs

namespace TUIKIT {

using namespace ftxui;

// =============================================================================
// THEME MANAGER DEFINITIONS
// =============================================================================

ThemeManager& ThemeManager::instance() {
    static ThemeManager instance;
    return instance;
}

void ThemeManager::set_theme(Theme theme) {
    switch (theme) {
        case Theme::Dark:
            current_theme_.primary = Color::Blue;
            current_theme_.secondary = Color::Cyan;
            current_theme_.success = Color::Green;
            current_theme_.warning = Color::Yellow;
            current_theme_.error = Color::Red;
            current_theme_.background = Color::Black;
            current_theme_.text = Color::White;
            current_theme_.border = Color::GrayDark;
            break;
        case Theme::Light:
            current_theme_.primary = Color::Blue;
            current_theme_.secondary = Color::Cyan;
            current_theme_.success = Color::Green;
            current_theme_.warning = Color::Yellow;
            current_theme_.error = Color::Red;
            current_theme_.background = Color::White;
            current_theme_.text = Color::Black;
            current_theme_.border = Color::GrayLight;
            break;
        case Theme::Monokai:
            current_theme_.primary = Color::Magenta;
            current_theme_.secondary = Color::Cyan;
            current_theme_.success = Color::GreenLight;
            current_theme_.warning = Color::Yellow;
            current_theme_.error = Color::RedLight;
            current_theme_.background = Color::Black;
            current_theme_.text = Color::White;
            current_theme_.border = Color::GrayDark;
            break;
        default:
            current_theme_ = AppTheme{}; // Default theme
    }
}

const AppTheme& ThemeManager::get_theme() const { return current_theme_; }

// =============================================================================
// CUSTOM WIDGETS DEFINITIONS
// =============================================================================

TUIKButton::TUIKButton(const std::string& text, EventCallback callback)
    : text_(text), callback_(callback) {
    component_ = Button(text_, [this] { 
        if (callback_) callback_(); 
    });
}

Component TUIKButton::get_component() { return component_; }

void TUIKButton::set_style(Color color) {
    color_ = color;
}

Element TUIKButton::render() {
    auto& theme = ThemeManager::instance().get_theme();
    return component_->Render() | color(color_.value_or(theme.primary));
}

TUIKInput::TUIKInput(const std::string& placeholder, const std::string& label)
    : placeholder_(placeholder), label_(label) {
    component_ = Input(&content_, placeholder_);
}

Component TUIKInput::get_component() { return component_; }

void TUIKInput::set_validator(ValidationFunc validator) {
    validator_ = validator;
}

std::string TUIKInput::get_text() const { return content_; }
void TUIKInput::set_text(const std::string& text) { content_ = text; }

bool TUIKInput::is_valid() const {
    return !validator_ || validator_(content_);
}

Element TUIKInput::render() {
    auto& theme = ThemeManager::instance().get_theme();
    auto input_element = component_->Render();
    
    if (!is_valid()) {
        input_element = input_element | color(theme.error);
    }
    
    if (!label_.empty()) {
        return vbox({
            text(label_) | color(theme.text),
            input_element | border | color(theme.border)
        });
    }
    
    return input_element | border | color(theme.border);
}

// PasswordInput::PasswordInput(const std::string& placeholder, const std::string& label)
//     : placeholder_(placeholder), label_(label) {
//     component_ = ftxui::Input(&content_, placeholder_, ftxui::InputOption::Password());
// }
//
// Component PasswordInput::get_component() {
//     return component_;
// }
//
// std::string PasswordInput::get_text() const {
//     return content_;
// }
//
// void PasswordInput::set_text(const std::string& text) {
//     content_ = text;
// }
//
// Element PasswordInput::render() {
//     auto& theme = ThemeManager::instance().get_theme();
//     auto input_element = component_->Render();
//
//     if (!label_.empty()) {
//         return ftxui::vbox({
//             ftxui::text(label_) | ftxui::color(theme.text),
//             input_element | ftxui::border | ftxui::color(theme.border)
//         });
//     }
//
//     return input_element | ftxui::border | ftxui::color(theme.border);
// }

TUIKTextArea::TUIKTextArea(const std::string& placeholder, const std::string& label, int height)
    : placeholder_(placeholder), label_(label), height_(height) {
    component_ = ftxui::Input(&content_, placeholder_);
}

Component TUIKTextArea::get_component() {
    return component_;
}

std::string TUIKTextArea::get_text() const {
    return content_;
}

void TUIKTextArea::set_text(const std::string& text) {
    content_ = text;
}

Element TUIKTextArea::render() {
    auto& theme = ThemeManager::instance().get_theme();
    auto text_area_element = component_->Render() | ftxui::size(ftxui::HEIGHT, ftxui::EQUAL, height_);

    if (!label_.empty()) {
        return ftxui::vbox({
            ftxui::text(label_) | ftxui::color(theme.text),
            text_area_element | ftxui::border | ftxui::color(theme.border)
        });
    }

    return text_area_element | ftxui::border | ftxui::color(theme.border);
}

StyledCheckbox::StyledCheckbox(const std::string& label, bool initial_state)
    : label_(label), checked_(initial_state) {
    component_ = Checkbox(label_, &checked_);
}

Component StyledCheckbox::get_component() { return component_; }
bool StyledCheckbox::is_checked() const { return checked_; }
void StyledCheckbox::set_checked(bool checked) { checked_ = checked; }

Element StyledCheckbox::render() {
    auto& theme = ThemeManager::instance().get_theme();
    return component_->Render() | color(theme.text);
}

StyledRadiobox::StyledRadiobox(const std::vector<std::string>& options, int initial_selected)
    : options_(options), selected_(initial_selected) {
    component_ = Radiobox(&options_, &selected_);
}

Component StyledRadiobox::get_component() { return component_; }
int StyledRadiobox::get_selected() const { return selected_; }
std::string StyledRadiobox::get_selected_text() const {
    return (selected_ >= 0 && static_cast<size_t>(selected_) < options_.size()) ? options_[selected_] : "";
}

Element StyledRadiobox::render() {
    auto& theme = ThemeManager::instance().get_theme();
    return component_->Render() | color(theme.text);
}

// DatePicker::DatePicker(int* year, int* month, int* day)
//     : year_(year), month_(month), day_(day) {
//     component_ = ftxui::DatePicker(year_, month_, day_);
// }
//
// Component DatePicker::get_component() {
//     return component_;
// }
//
// Element DatePicker::render() {
//     auto& theme = ThemeManager::instance().get_theme();
//     return component_->Render() | ftxui::color(theme.text);
// }

TUIKTreeView::TUIKTreeView(TreeNode root_node)
    : root_node_(std::move(root_node)) {
    component_ = build_tree_component(root_node_);
}

Component TUIKTreeView::get_component() {
    return component_;
}

Element TUIKTreeView::render() {
    return component_->Render();
}

Component TUIKTreeView::build_tree_component(TreeNode& node) {
    std::vector<Component> children_components;
    for (auto& child : node.children) {
        children_components.push_back(build_tree_component(child));
    }

    auto renderer = ftxui::Renderer([&node, children_components] {
        ftxui::Element label = ftxui::text(node.label);
        if (!node.children.empty()) {
            label = ftxui::hbox({(node.is_expanded ? ftxui::text("▼ ") : ftxui::text("► ")) | ftxui::color(ftxui::Color::Blue) | ftxui::bold, label});
        }

        if (node.is_expanded) {
            ftxui::Elements elements;
            elements.push_back(label);
            for (const auto& child_comp : children_components) {
                elements.push_back(ftxui::hbox({ftxui::text("  "), child_comp->Render()}));
            }
            return ftxui::vbox(elements);
        } else {
            return label;
        }
    });

    return ftxui::CatchEvent(renderer, [&](ftxui::Event event) {
        if (event.is_mouse() && event.mouse().button == ftxui::Mouse::Button::Left && event.mouse().motion == ftxui::Mouse::Motion::Released) {
            if (!node.children.empty()) {
                node.is_expanded = !node.is_expanded;
                return true;
            }
        }
        return false;
    });
}

StyledMenu::StyledMenu(const std::vector<std::string>& options, EventCallback on_select)
    : options_(options), selected_(0), on_select_(on_select) {
    component_ = Menu(&options_, &selected_);
    if (on_select_) {
        component_ = CatchEvent(component_, [&](Event event) {
            if (event == Event::Return) {
                on_select_();
                return true;
            }
            return false;
        });
    }
}

Component StyledMenu::get_component() { return component_; }
int StyledMenu::get_selected() const { return selected_; }
std::string StyledMenu::get_selected_text() const {
    return (selected_ >= 0 && static_cast<size_t>(selected_) < options_.size()) ? options_[selected_] : "";
}

void StyledMenu::set_on_select(EventCallback callback) { on_select_ = callback; }

void StyledMenu::set_options(const std::vector<std::string>& options) {
    options_ = options;
    selected_ = 0; // Reset selection when options change
    component_ = Menu(&options_, &selected_);
    if (on_select_) {
        component_ = CatchEvent(component_, [&](Event event) {
            if (event == Event::Return) {
                on_select_();
                return true;
            }
            return false;
        });
    }
}

Element StyledMenu::render() {
    auto& theme = ThemeManager::instance().get_theme();
    return component_->Render() | border | color(theme.border);
}

// StyledTable::StyledTable(const std::vector<std::vector<std::string>>& data, const std::vector<std::string>& headers)
//     : data_(data), headers_(headers) {
//     std::vector<std::vector<ftxui::Element>> table_elements;
//
//     if (!headers_.empty()) {
//         std::vector<ftxui::Element> header_elements;
//         for (const auto& header : headers_) {
//             header_elements.push_back(ftxui::text(header) | ftxui::bold | ftxui::center);
//         }
//         table_elements.push_back(header_elements);
//     }
//
//     for (const auto& row : data_) {
//         std::vector<ftxui::Element> row_elements;
//         for (const auto& cell : row) {
//             row_elements.push_back(ftxui::text(cell));
//         }
//         table_elements.push_back(row_elements);
//     }
//
//     component_ = ftxui::Table(table_elements);
// }
//
// Component StyledTable::get_component() {
//     return component_;
// }
//
// Element StyledTable::render() {
//     auto& theme = ThemeManager::instance().get_theme();
//     return component_->Render() | ftxui::border | ftxui::color(theme.border);
// }

ProgressBar::ProgressBar(float initial_value, const std::string& label)
    : value_(initial_value), label_(label) {}

void ProgressBar::set_value(float value) { 
    value_ = std::clamp(value, 0.0f, 1.0f); 
}

float ProgressBar::get_value() const { return value_; }

void ProgressBar::set_label(const std::string& label) { label_ = label; }

Element ProgressBar::render() {
    auto& theme = ThemeManager::instance().get_theme();
    auto progress_text = std::to_string(static_cast<int>(value_ * 100)) + "%";
    
    if (!label_.empty()) {
        progress_text = label_ + " " + progress_text;
    }
    
    return vbox({
        text(progress_text) | color(theme.text),
        gauge(value_) | color(theme.primary)
    });
}

// ActivityIndicator::ActivityIndicator() {
//     component_ = ftxui::Spinner();
// }
//
// Component ActivityIndicator::get_component() {
//     return component_;
// }
//
// Element ActivityIndicator::render() {
//     auto& theme = ThemeManager::instance().get_theme();
//     return component_->Render() | ftxui::color(theme.primary);
// }

TUIKSlider::TUIKSlider(const std::string& label, float* value, float min, float max, float increment)
    : label_(label), value_(value), min_(min), max_(max), increment_(increment) {
    component_ = ftxui::Slider(label_, value_, min_, max_, increment_);
}

Component TUIKSlider::get_component() {
    return component_;
}

Element TUIKSlider::render() {
    auto& theme = ThemeManager::instance().get_theme();
    return component_->Render() | color(theme.text);
}

// =============================================================================
// CONTAINERS AND LAYOUTS DEFINITIONS
// =============================================================================

TUIKPanel::TUIKPanel(const std::string& title, bool show_border)
    : title_(title), show_border_(show_border) {
    internal_container_ = Container::Vertical({}); 
    component_ = Renderer(internal_container_, [this] {
        auto& theme = ThemeManager::instance().get_theme();
        auto content = internal_container_->Render();
        
        if (show_border_) {
            content = content | border | color(theme.border);
            
            if (!title_.empty()) {
                content = content | borderWith(Pixel()) | color(theme.border);
                content = vbox({
                    text(" " + title_ + " ") | color(theme.primary) | bold,
                    content
                });
            }
        }
        return content;
    });
}

void TUIKPanel::add_child(Component child) {
    internal_container_->Add(child);
}

void TUIKPanel::set_title(const std::string& title) { title_ = title; }
Component TUIKPanel::get_component() { return component_; }

TUIKScrollableContainer::TUIKScrollableContainer(Component content, int height)
    : content_(content), height_(height) {
    component_ = content_ | ftxui::vscroll_indicator | ftxui::frame | ftxui::size(ftxui::HEIGHT, ftxui::EQUAL, height_);
}

Component TUIKScrollableContainer::get_component() {
    return component_;
}

TUIKTabbedPane::TUIKTabbedPane(const std::vector<std::string>& tab_titles, const std::vector<Component>& tab_contents)
    : tab_titles_(tab_titles), tab_contents_(tab_contents) {
    component_ = ftxui::Container::Vertical({
        ftxui::Menu(&tab_titles_, &selected_tab_),
        ftxui::Container::Tab(tab_contents_, &selected_tab_)
    });
}

Component TUIKTabbedPane::get_component() {
    return component_;
}

int TUIKTabbedPane::get_selected_tab() const {
    return selected_tab_;
}

void TUIKTabbedPane::set_selected_tab(int index) {
    selected_tab_ = index;
}

// =============================================================================
// PREDEFINED DIALOGS DEFINITIONS
// =============================================================================

void MessageDialog::show_info(Application& app, const std::string& message, const std::string& title) {
    auto ok_button = Button("OK", [&app] { app.close_modal(); });
    auto dialog = Renderer(ok_button, [=] {
        return vbox({
            text(title) | bold,
            separator(),
            paragraph(message),
            separator(),
            ok_button->Render() | center
        }) | border | size(WIDTH, LESS_THAN, 50) | center;
    });
    app.show_modal(dialog);
}

void MessageDialog::show_warning(Application& app, const std::string& message, const std::string& title) {
    show_info(app, message, title);
}

void MessageDialog::show_error(Application& app, const std::string& message, const std::string& title) {
    show_info(app, message, title);
}

void MessageDialog::confirm(Application& app, const std::string& message, std::function<void(bool)> callback, const std::string& title) {
    auto ok_button = Button("OK", [&app, callback] {
        app.close_modal();
        if (callback) callback(true);
    });
    auto cancel_button = Button("Cancel", [&app, callback] {
        app.close_modal();
        if (callback) callback(false);
    });
    auto buttons = Container::Horizontal({ok_button, cancel_button});
    auto dialog = Renderer(buttons, [=] {
        return vbox({
            text(title) | bold,
            separator(),
            paragraph(message),
            separator(),
            buttons->Render() | center
        }) | border | size(WIDTH, LESS_THAN, 50) | center;
    });
    app.show_modal(dialog);
}

// =============================================================================
// MAIN APPLICATION DEFINITIONS
// =============================================================================

Application::Application(const std::string& title)
    : title_(title), screen_(ScreenInteractive::Fullscreen()) {
    ThemeManager::instance().set_theme(Theme::Default);
}

void Application::set_theme(Theme theme) {
    ThemeManager::instance().set_theme(theme);
}

void Application::add_page(const std::string& name, Component page) {
    page_names_.push_back(name);
    pages_.push_back(page);
}

void Application::show_status_bar(bool show) { show_status_bar_ = show; }
void Application::set_status_text(const std::string& status) { status_text_ = status; }

void Application::run() {
    if (pages_.empty()) {
        std::cerr << "Error: No pages have been added to the application." << std::endl;
        throw std::runtime_error("No pages have been added to the application.");
    }

    // These components are the basic interactive elements of the application.
    auto menu = ftxui::Menu(&page_names_, &active_page_);
    auto page_container = ftxui::Container::Tab(pages_, &active_page_);

    // The main interactive layout (menu + page content).
    auto main_layout_component = ftxui::Container::Horizontal({
        menu,
        page_container,
    });

    // The component that will be rendered by the screen. It includes decorations and the modal.
    auto root_renderable_component = ftxui::Renderer(main_layout_component, [&] {
        auto& theme = ThemeManager::instance().get_theme();

        ftxui::Element page_view = ftxui::hbox({
            menu->Render() | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 20) | ftxui::border | ftxui::color(theme.border),
            ftxui::separator(),
            page_container->Render() | ftxui::flex,
        });

        ftxui::Element decorated_view = ftxui::vbox({
            ftxui::text(title_) | ftxui::bold | ftxui::center | ftxui::color(theme.primary),
            ftxui::separator(),
            page_view | ftxui::flex,
        });

        if (show_status_bar_) {
            decorated_view = ftxui::vbox({
                decorated_view | ftxui::flex,
                ftxui::separator(),
                ftxui::text(status_text_) | ftxui::color(theme.text),
            });
        }

        // Superposition du dialogue modal.
        if (show_modal_ && modal_) {
            return ftxui::dbox({decorated_view, modal_->Render() | ftxui::clear_under | ftxui::center});
        }

        return decorated_view;
    });

    // The final component passed to screen.Loop must handle events and focus.
    // It overlays the main interface and the modal.
    std::vector<Component> children;
    children.push_back(root_renderable_component);
    if (show_modal_ && modal_) {
        children.push_back(ftxui::Maybe(modal_, &show_modal_));
    }
    auto final_interactive_root = ftxui::Container::Vertical(children);

    // The event dispatcher directs events to the modal if active,
    // otherwise to the main interactive layout.
    auto event_dispatcher = ftxui::CatchEvent(final_interactive_root, [&](ftxui::Event event) {
        if (show_modal_) {
            return modal_->OnEvent(event);
        } else {
            // Forward event to the currently active page if it's a ComponentBase
            if (active_page_ >= 0 && active_page_ < pages_.size()) {
                ftxui::Component active_page_component = pages_[active_page_];
                // std::cerr << "[Application] Dispatching event to active page (index: " << active_page_ << ")" << std::endl;
                if (active_page_component->OnEvent(event)) {
                    return true; // Event handled by the active page
                }
            }
            return main_layout_component->OnEvent(event); // Fallback to main layout
        }
    });

    screen_.Loop(event_dispatcher);
}

void Application::exit() {
    screen_.Exit();
}

void Application::show_modal(Component modal) {
    modal_ = modal;
    show_modal_ = true;
}

void Application::close_modal() {
    show_modal_ = false;
    modal_ = nullptr;
}

void Application::show_message(const std::string& message, const std::string& title) {
    MessageDialog::show_info(*this, message, title.empty() ? "Information" : title);
}

void Application::show_error(const std::string& message, const std::string& title) {
    MessageDialog::show_error(*this, message, title.empty() ? "Error" : title);
}

void Application::confirm(const std::string& message, std::function<void(bool)> callback, const std::string& title) {
    MessageDialog::confirm(*this, message, callback, title.empty() ? "Confirmation" : title);
}

void Application::request_redraw() {
    screen_.PostEvent(ftxui::Event::Custom);
}

    void Application::PostEvent(ftxui::Event event) {
        std::cerr << "[Application] Posting event: input='" << event.input() << "'" << std::endl;
        screen_.PostEvent(event);
    }

void Application::set_active_page(int index) {
    if (index >= 0 && index < pages_.size()) {
        active_page_ = index;
    }
}

void Notification::show(Application& app, const std::string& message, const std::string& title, int duration_ms) {
    auto notification_content = ftxui::vbox({
        ftxui::text(title) | ftxui::bold,
        ftxui::separator(),
        ftxui::paragraph(message)
    });

    auto notification_element = notification_content | ftxui::border | ftxui::size(ftxui::WIDTH, ftxui::LESS_THAN, 50);

    // Create a container that pushes the notification to the bottom-right
    auto modal_container = ftxui::Renderer([notification_element] {
        std::vector<ftxui::Element> hbox_elements;
        hbox_elements.push_back(ftxui::text("") | ftxui::flex);
        hbox_elements.push_back(notification_element);

        std::vector<ftxui::Element> vbox_elements;
        vbox_elements.push_back(ftxui::text("") | ftxui::flex);
        vbox_elements.push_back(ftxui::hbox(hbox_elements));

        return ftxui::vbox(vbox_elements);
    });

    app.show_modal(modal_container);

    // Close the notification after duration_ms
    std::thread([&app, duration_ms]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(duration_ms));
        app.close_modal();
    }).detach();
}

// =============================================================================
// BUILDERS AND UTILITIES DEFINITIONS
// =============================================================================

std::unique_ptr<TUIKButton> WidgetBuilder::create_button(const std::string& text, EventCallback callback) {
    return std::make_unique<TUIKButton>(text, callback);
}

std::unique_ptr<TUIKInput> WidgetBuilder::create_input(const std::string& placeholder, const std::string& label) {
    return std::make_unique<TUIKInput>(placeholder, label);
}

// std::unique_ptr<PasswordInput> WidgetBuilder::create_password_input(const std::string& placeholder, const std::string& label) {
//     return std::make_unique<PasswordInput>(placeholder, label);
// }

std::unique_ptr<TUIKTextArea> WidgetBuilder::create_text_area(const std::string& placeholder, const std::string& label, int height) {
    return std::make_unique<TUIKTextArea>(placeholder, label, height);
}

// std::unique_ptr<StyledTable> WidgetBuilder::create_table(const std::vector<std::vector<std::string>>& data, const std::vector<std::string>& headers) {
//     return std::make_unique<StyledTable>(data, headers);
// }

std::unique_ptr<TUIKTreeView> WidgetBuilder::create_tree_view(TreeNode root_node) {
    return std::make_unique<TUIKTreeView>(std::move(root_node));
}

std::unique_ptr<TUIKTabbedPane> WidgetBuilder::create_tabbed_pane(const std::vector<std::string>& tab_titles, const std::vector<Component>& tab_contents) {
    return std::make_unique<TUIKTabbedPane>(tab_titles, tab_contents);
}

std::unique_ptr<TUIKScrollableContainer> WidgetBuilder::create_scrollable_container(Component content, int height) {
    return std::make_unique<TUIKScrollableContainer>(content, height);
}

std::unique_ptr<TUIKPanel> WidgetBuilder::create_panel(const std::string& title, bool border) {
    return std::make_unique<TUIKPanel>(title, border);
}

Component WidgetBuilder::create_form_field(const std::string& label, Component input) {
    return Renderer([=] {
        auto& theme = ThemeManager::instance().get_theme();
        return vbox({
            text(label) | color(theme.text),
            input->Render()
        });
    });
}

Component WidgetBuilder::create_button_bar(const std::vector<std::pair<std::string, EventCallback>>& buttons) {
    std::vector<Component> button_components;
    
    for (const auto& [text, callback] : buttons) {
        button_components.push_back(Button(text, callback));
    }
    
    return Container::Horizontal(button_components);
}

std::unique_ptr<TUIKSlider> WidgetBuilder::create_slider(const std::string& label, float* value, float min, float max, float increment) {
    return std::make_unique<TUIKSlider>(label, value, min, max, increment);
}

// std::unique_ptr<ActivityIndicator> WidgetBuilder::create_activity_indicator() {
//     return std::make_unique<ActivityIndicator>();
// }

// std::unique_ptr<DatePicker> WidgetBuilder::create_date_picker(int* year, int* month, int* day) {
//     return std::make_unique<DatePicker>(year, month, day);
// }

} // namespace TUIKIT
