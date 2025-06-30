#include "tuikit/widgets/TUIForm.h"
#include "tuikit/core/TUIStyle.h"
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>

namespace TUIKIT {

TUIForm::TUIForm(TUIWidget* parent) : TUIWidget(parent) {}

void TUIForm::addField(const std::string& label, std::shared_ptr<TUIWidget> field) {
    fields_.push_back({label, field});
}

ftxui::Component TUIForm::get_ftxui_component() {
    auto container = ftxui::Container::Vertical({});
    for (auto& field : fields_) {
        auto labeled_field = ftxui::Container::Horizontal({
            ftxui::Renderer([=] { 
                auto& theme = TUIStyle::instance().currentTheme();
                return ftxui::text(field.first) | ftxui::color(theme.text);
            }),
            field.second->get_ftxui_component()
        });
        container->Add(labeled_field);
    }
    return container;
}

} // namespace TUIKIT