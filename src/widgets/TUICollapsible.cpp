#include "tuikit/widgets/TUICollapsible.h"
#include <ftxui/component/component.hpp>

namespace TUIKIT {

TUICollapsible::TUICollapsible(const std::string& title, std::shared_ptr<TUIWidget> content, TUIWidget* /*parent*/)
    : TUIWidget(ftxui::Collapsible(title, content->get_ftxui_component())), title_(title), content_(content) {
}

} // namespace TUIKIT