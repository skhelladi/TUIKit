#include "tuikit/widgets/TUIButton.h"
#include "tuikit/core/TUIStyle.h"
#include <ftxui/component/component.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/dom/elements.hpp>

namespace TUIKIT {

TUIButton::TUIButton(const std::string& text, TUIWidget* /*parent*/)
    : TUIWidget(ftxui::Component()), original_text_(text), text_(text) {
    rebuildComponent();
}

void TUIButton::onClick(OnClickedCallback callback) {
    on_clicked_ = callback;
}

void TUIButton::setIcon(const std::string& icon) {
    icon_ = icon;
    text_ = icon_ + " " + original_text_;
    rebuildComponent();
}

void TUIButton::setText(const std::string& text) {
    original_text_ = text;
    text_ = icon_ + " " + original_text_;
    rebuildComponent();
}

void TUIButton::setEnabled(bool enabled) {
    if (enabled_ == enabled) return;
    enabled_ = enabled;
    rebuildComponent();
    requestRedraw();
}

bool TUIButton::isEnabled() const {
    return enabled_;
}

const std::string& TUIButton::icon() const {
    return icon_;
}

void TUIButton::rebuildComponent() {
    auto base = ftxui::Button(&text_, [this] {
        if (on_clicked_) on_clicked_();
    });
    if (enabled_) {
        component_ = base;
    } else {
        // Disabled: render dimmed and swallow activation events.
        component_ = ftxui::Renderer(base, [base] {
            return base->Render() | ftxui::dim;
        });
        component_ = ftxui::CatchEvent(component_, [](ftxui::Event event) {
            return event.is_mouse() || event == ftxui::Event::Return
                || event == ftxui::Event::Character(" ");
        });
    }
}

} // namespace TUIKIT
