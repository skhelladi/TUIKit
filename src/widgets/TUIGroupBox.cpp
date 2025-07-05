#include "tuikit/widgets/TUIGroupBox.h"
#include "tuikit/core/TUIStyle.h"
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>

namespace TUIKIT {

TUIGroupBox::TUIGroupBox(const std::string& title, std::shared_ptr<TUIWidget> content_widget, bool show_border, TUIWidget* /*parent*/)
    : TUIWidget(ftxui::Renderer([=] { return ftxui::Element(); })), title_(title), content_widget_(content_widget), show_border_(show_border) {
    // Initial render setup
    component_ = ftxui::Renderer(content_widget_->get_ftxui_component(), [this] {
        auto& theme = TUIStyle::instance().currentTheme();
        ftxui::Element content_element = content_widget_->get_ftxui_component()->Render();

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
    // Re-render the component to reflect the new content
    component_ = ftxui::Renderer(content_widget_->get_ftxui_component(), [this] {
        auto& theme = TUIStyle::instance().currentTheme();
        ftxui::Element content_element = content_widget_->get_ftxui_component()->Render();

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

void TUIGroupBox::setShowBorder(bool show_border) {
    show_border_ = show_border;
}

} // namespace TUIKIT