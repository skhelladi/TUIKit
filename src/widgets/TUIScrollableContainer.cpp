#include "tuikit/widgets/TUIScrollableContainer.h"
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/component/event.hpp>

namespace TUIKIT {

TUIScrollableContainer::TUIScrollableContainer(std::shared_ptr<TUIWidget> content, int height, TUIWidget* /*parent*/)
    : TUIWidget(ftxui::Component()), content_(content), height_(height) {
    // frame + vscroll_indicator on the inner component: the frame clips the
    // overflow and scrolls it (mouse wheel included), the indicator draws the
    // scrollbar on the right edge whenever the content is taller than the
    // viewport. Order matters: vscroll_indicator must wrap the content
    // BEFORE frame (i.e. `content | vscroll_indicator | frame`), so the
    // indicator sees the full content box while the frame gives it the
    // visible stencil to compare against. The other way around, the
    // indicator only sees the clipped viewport and never draws.
    auto inner = content->get_ftxui_component();
    if (height > 0) {
        // Fixed mode: a viewport of exactly `height` rows.
        auto renderer = ftxui::Renderer(inner, [inner, height] {
            return inner->Render()
                | ftxui::vscroll_indicator
                | ftxui::frame
                | ftxui::size(ftxui::HEIGHT, ftxui::EQUAL, height);
        });
        component_ = renderer;
    } else {
        // Flexible mode: the container takes the full height its parent
        // layout gives it (e.g. the whole Properties pane), whatever the
        // pane's actual size, and scrolls the overflow.
        auto renderer = ftxui::Renderer(inner, [inner] {
            return inner->Render()
                | ftxui::vscroll_indicator
                | ftxui::frame
                | ftxui::flex;
        });
        component_ = renderer;
    }
}

} // namespace TUIKIT
