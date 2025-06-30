#include "tuikit/widgets/TUIGroupBox.h"
#include "tuikit/core/TUIStyle.h"
#include <ftxui/component/component.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>

namespace TUIKIT {

TUIGroupBox::TUIGroupBox(const std::string& title, std::shared_ptr<TUIWidget> content_widget, bool show_border, TUIWidget* parent)
    : TUIWidget(parent), title_(title), content_widget_(content_widget), show_border_(show_border) {
}

ftxui::Component TUIGroupBox::get_ftxui_component() {
    // Create a container for the content widget
    // This container will manage the focus and events for the content_widget_
    auto content_container = ftxui::Container::Vertical({content_widget_->get_ftxui_component()});

    // Return a Renderer that applies the styling to the rendered content_container
    return ftxui::Renderer(content_container, [this, content_container] {
        auto& theme = TUIStyle::instance().currentTheme();
        ftxui::Element content_element = content_container->Render(); // Render the container

        if (show_border_) {
            content_element = content_element | ftxui::border | ftxui::color(theme.border);
            if (!title_.empty()) {
                content_element = ftxui::vbox({
                    ftxui::text(" " + title_ + " ") | ftxui::color(theme.primary) | ftxui::bold,
                    content_element
                });
            }
        }
        return content_element;
    });
}

void TUIGroupBox::setTitle(const std::string& title) {
    title_ = title;
}

void TUIGroupBox::setContentWidget(std::shared_ptr<TUIWidget> content_widget) {
    content_widget_ = content_widget;
}

void TUIGroupBox::setShowBorder(bool show_border) {
    show_border_ = show_border;
}

} // namespace TUIKIT