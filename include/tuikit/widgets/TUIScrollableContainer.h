#ifndef TUIKIT_TUISCROLLABLECONTAINER_H
#define TUIKIT_TUISCROLLABLECONTAINER_H

#include "tuikit/core/TUIWidget.h"
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>

namespace TUIKIT {

class TUIScrollableContainer : public TUIWidget {
public:
    TUIScrollableContainer(std::shared_ptr<TUIWidget> content, int height, TUIWidget* parent = nullptr);
    ~TUIScrollableContainer() override = default;

private:
    std::shared_ptr<TUIWidget> content_;
    int height_;
};

} // namespace TUIKIT

#endif // TUIKIT_TUISCROLLABLECONTAINER_H
