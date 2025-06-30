#pragma once
#include <ftxui/component/component.hpp>
namespace TUIKIT {
class TUIWidget {
public:
    TUIWidget(TUIWidget* parent = nullptr);
    virtual ~TUIWidget();
    virtual ftxui::Component get_ftxui_component() = 0;
};
}
