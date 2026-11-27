#ifndef TUIKIT_TUISCROLLABLECONTAINER_H
#define TUIKIT_TUISCROLLABLECONTAINER_H

#include "tuikit/core/TUIWidget.h"
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>

namespace TUIKIT {

class TUIScrollableContainer : public TUIWidget {
public:
    // Two height modes:
    //   - height > 0: fixed viewport of exactly `height` rows (the content
    //     scrolls inside that window).
    //   - height <= 0 (default): flexible — the container takes the full
    //     height the parent layout gives it (| flex) and scrolls whatever
    //     overflows, with the scrollbar rendered on the right edge.
    TUIScrollableContainer(std::shared_ptr<TUIWidget> content, int height = 0, TUIWidget* parent = nullptr);
    ~TUIScrollableContainer() override = default;

private:
    std::shared_ptr<TUIWidget> content_;
    [[maybe_unused]] int height_;
    [[maybe_unused]] int scroll_position_ = 0;
};

} // namespace TUIKIT

#endif // TUIKIT_TUISCROLLABLECONTAINER_H
