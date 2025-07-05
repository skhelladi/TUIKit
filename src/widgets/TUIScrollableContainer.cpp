#include "tuikit/widgets/TUIScrollableContainer.h"
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/component/event.hpp>
#include <iostream> // For debug output

namespace TUIKIT {

TUIScrollableContainer::TUIScrollableContainer(std::shared_ptr<TUIWidget> content, int height, TUIWidget* /*parent*/)
    : TUIWidget(content->get_ftxui_component() | ftxui::vscroll_indicator | ftxui::frame | ftxui::size(ftxui::HEIGHT, ftxui::EQUAL, height)), content_(content), height_(height) {
}

} // namespace TUIKIT