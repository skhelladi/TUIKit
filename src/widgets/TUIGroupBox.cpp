#include "tuikit/widgets/TUIGroupBox.h"
#include "tuikit/core/TUIStyle.h"
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>

namespace TUIKIT {

TUIGroupBox::TUIGroupBox(const std::string& title, std::shared_ptr<TUIWidget> content_widget, bool show_border, TUIWidget* /*parent*/)
    : TUIWidget(ftxui::Renderer([=] { return ftxui::Element(); })), title_(title), content_widget_(content_widget), show_border_(show_border) {
    rebuildRenderer();
}

ftxui::Element TUIGroupBox::renderContent() const {
    auto& theme = TUIStyle::instance().currentTheme();
    ftxui::Element content_element =
        content_widget_ ? content_widget_->get_ftxui_component()->Render() : ftxui::text("");
    if (!show_border_) {
        return content_element;
    }
    content_element = content_element | ftxui::color(theme.border);
    if (title_.empty()) {
        return content_element | ftxui::border;
    }
    // Title drawn inside the frame, so the box reads like a GUI group box.
    return ftxui::window(
        ftxui::text(" " + title_ + " ") | ftxui::color(theme.primary) | ftxui::bold,
        content_element);
}

void TUIGroupBox::rebuildRenderer() {
    if (content_widget_ && content_widget_->get_ftxui_component()) {
        component_ = ftxui::Renderer(content_widget_->get_ftxui_component(),
                                     [this] { return renderContent(); });
    } else {
        component_ = ftxui::Renderer([this] { return renderContent(); });
    }
    requestRedraw();
}

void TUIGroupBox::setTitle(const std::string& title) {
    title_ = title;
    requestRedraw();
}

void TUIGroupBox::setContentWidget(std::shared_ptr<TUIWidget> content_widget) {
    content_widget_ = content_widget;
    rebuildRenderer();
}

void TUIGroupBox::setShowBorder(bool show_border) {
    show_border_ = show_border;
    requestRedraw();
}

} // namespace TUIKIT
