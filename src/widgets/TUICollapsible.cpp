#include "tuikit/widgets/TUICollapsible.h"
#include <ftxui/component/component.hpp>

namespace TUIKIT {

TUICollapsible::TUICollapsible(const std::string& title, std::shared_ptr<TUIWidget> content, TUIWidget* parent)
    : TUIWidget(parent), title_(title), content_(content) {
}

ftxui::Component TUICollapsible::get_ftxui_component() {
    return ftxui::Collapsible(title_, content_->get_ftxui_component());
}

} // namespace TUIKIT