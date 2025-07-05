#include "tuikit/widgets/TUIForm.h"
#include "tuikit/core/TUIStyle.h"
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>

namespace TUIKIT {

TUIForm::TUIForm(TUIWidget* /*parent*/)
    : TUIWidget(ftxui::Container::Vertical({})) {
}

void TUIForm::addField(const std::string& label, std::shared_ptr<TUIWidget> field) {
    fields_.push_back({label, field});
    component_->Add(ftxui::Container::Horizontal({
        ftxui::Renderer([=] { 
            auto& theme = TUIStyle::instance().currentTheme();
            return ftxui::text(label) | ftxui::color(theme.text);
        }),
        field->get_ftxui_component()
    }));
}

} // namespace TUIKIT