#pragma once
#include <ftxui/component/component.hpp>

namespace TUIKIT {

class TUIWidget {
public:
    TUIWidget(ftxui::Component component);
    virtual ~TUIWidget() = default;

    ftxui::Component get_ftxui_component() const { return component_; }

protected:
    ftxui::Component component_;
};

} // namespace TUIKIT