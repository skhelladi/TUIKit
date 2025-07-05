#include "tuikit/widgets/TUITextArea.h"
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>

namespace TUIKIT {

TUITextArea::TUITextArea(const std::string& placeholder, const std::string& label, int height, TUIWidget* /*parent*/)
    : TUIWidget(ftxui::Input(&value_, placeholder)), value_(""), placeholder_(placeholder), label_(label), height_(height) {
    component_ = ftxui::Renderer(component_, [this] {
        auto text_area_element = component_->Render() | ftxui::size(ftxui::HEIGHT, ftxui::EQUAL, height_);
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

} // namespace TUIKIT