#pragma once

#include <ftxui/component/component.hpp>
#include <memory>

namespace TUIKIT {

class TUIWidget {
public:
    TUIWidget(ftxui::Component component = ftxui::Renderer([]{ return ftxui::text("Not implemented component"); }));
    virtual ~TUIWidget() = default;

    ftxui::Component get_ftxui_component() const { return component_; }

protected:
    ftxui::Component component_;
};

}