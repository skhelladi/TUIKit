#include "tuikit/widgets/TUITextArea.h"
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/component/event.hpp>

namespace TUIKIT {

TUITextArea::TUITextArea(const std::string& placeholder, const std::string& label, int height, TUIWidget* /*parent*/)
    : value_(""), placeholder_(placeholder), label_(label), height_(height) {
    input_ftxui_component_ = ftxui::Input(&value_, placeholder);
    component_ = ftxui::Renderer(input_ftxui_component_, [this] {
        auto text_area_element = input_ftxui_component_->Render() | ftxui::size(ftxui::HEIGHT, ftxui::EQUAL, height_) | ftxui::vscroll_indicator | ftxui::frame;
        if (!label_.empty()) {
            return ftxui::vbox({
                ftxui::text(label_),
                text_area_element | ftxui::border
            });
        }
        return text_area_element | ftxui::border;
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
    if (read_only_) {
        component_ = ftxui::Renderer(input_ftxui_component_, [this, input_ftxui_component_ = input_ftxui_component_]() {
            auto text_area_element = input_ftxui_component_->Render() | ftxui::size(ftxui::HEIGHT, ftxui::EQUAL, height_) | ftxui::color(ftxui::Color::GrayDark) | ftxui::bgcolor(ftxui::Color::Black) | ftxui::vscroll_indicator | ftxui::frame;
            if (!label_.empty()) {
                ftxui::Elements elements;
                elements.push_back(ftxui::text(label_));
                elements.push_back(text_area_element | ftxui::border);
                return ftxui::vbox(std::move(elements));
            }
            return text_area_element | ftxui::border;
        });
        component_ = ftxui::CatchEvent(component_, [](ftxui::Event event) {
            // Block character input events, allow others (like navigation, mouse scroll)
            return event.is_character() || event == ftxui::Event::Backspace || event == ftxui::Event::Delete;
        });
    } else {
        component_ = ftxui::Renderer(input_ftxui_component_, [this, input_ftxui_component_ = input_ftxui_component_]() {
            auto text_area_element = input_ftxui_component_->Render() | ftxui::size(ftxui::HEIGHT, ftxui::EQUAL, height_) | ftxui::vscroll_indicator | ftxui::frame;
            if (!label_.empty()) {
                ftxui::Elements elements;
                elements.push_back(ftxui::text(label_));
                elements.push_back(text_area_element | ftxui::border);
                return ftxui::vbox(std::move(elements));
            }
            return text_area_element | ftxui::border;
        });
    }
}

bool TUITextArea::isReadOnly() const {
    return read_only_;
}

} // namespace TUIKIT