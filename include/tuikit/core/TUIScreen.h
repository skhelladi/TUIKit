#ifndef TUIKIT_TUISCREEN_H
#define TUIKIT_TUISCREEN_H

#include "tuikit/core/TUIWidget.h"
#include <ftxui/component/component.hpp>

namespace TUIKIT {

class TUIScreen : public TUIWidget {
public:
    TUIScreen(TUIWidget* parent = nullptr);
    ~TUIScreen() override = default;

    void setLayout(std::shared_ptr<TUIWidget> layout_widget);
    ftxui::Component get_ftxui_component() override;

private:
    std::shared_ptr<TUIWidget> layout_widget_ = nullptr;
};

} // namespace TUIKIT

#endif // TUIKIT_TUISCREEN_H
