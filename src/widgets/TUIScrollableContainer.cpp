#include "tuikit/widgets/TUIScrollableContainer.h"
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/component/event.hpp>

namespace TUIKIT {

TUIScrollableContainer::TUIScrollableContainer(std::shared_ptr<TUIWidget> content, int height, TUIWidget* /*parent*/)
    : TUIWidget(ftxui::Component()), content_(content), height_(height) {
    // On applique frame et vscroll_indicator sur le composant interne (menu ou autre)
    auto menu_component = content->get_ftxui_component();
    auto renderer = ftxui::Renderer(menu_component, [menu_component, height] {
        return menu_component->Render()
            | ftxui::frame
            | ftxui::vscroll_indicator
            | ftxui::size(ftxui::HEIGHT, ftxui::EQUAL, height);
    });
    component_ = renderer;
}

} // namespace TUIKIT
