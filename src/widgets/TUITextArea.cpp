#include "tuikit/widgets/TUITextArea.h"
#include "tuikit/core/TUIStyle.h"
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/component/event.hpp>

namespace TUIKIT {

TUITextArea::TUITextArea(const std::string& placeholder, const std::string& label, int height, TUIWidget* /*parent*/)
    : value_(""), placeholder_(placeholder), label_(label), height_(height) {
    input_ftxui_component_ = ftxui::Input(&value_, placeholder);
    component_ = buildRenderer();
}

ftxui::Component TUITextArea::buildRenderer() const {
    return ftxui::Renderer(input_ftxui_component_, [this] {
        // Read-only fields never show the focus highlight.
        const bool focused = !read_only_ && input_ftxui_component_->Focused();
        auto text_area_element = input_ftxui_component_->Render() | ftxui::size(ftxui::HEIGHT, ftxui::EQUAL, height_) | ftxui::vscroll_indicator | ftxui::frame;
        auto framed = TUIStyle::fieldFrame(std::move(text_area_element), focused) | ftxui::xflex;
        if (!label_.empty()) {
            auto& theme = TUIStyle::instance().currentTheme();
            return ftxui::vbox({
                ftxui::text(label_) | ftxui::color(theme.text),
                std::move(framed)
            });
        }
        return framed;
    });
}

void TUITextArea::onChange(OnChangeCallback cb) {
    on_change_ = cb;
}

std::string TUITextArea::text() const {
    return value_;
}

void TUITextArea::setText(const std::string& text) {
    value_ = text;
}

void TUITextArea::setReadOnly(bool read_only) {
    read_only_ = read_only;
    component_ = buildRenderer();
    if (read_only_) {
        component_ = ftxui::CatchEvent(component_, [](ftxui::Event event) {
            // Block character input events, allow others (like navigation, mouse scroll)
            return event.is_character() || event == ftxui::Event::Backspace || event == ftxui::Event::Delete;
        });
    }
    requestRedraw();
}

bool TUITextArea::isReadOnly() const {
    return read_only_;
}

} // namespace TUIKIT
