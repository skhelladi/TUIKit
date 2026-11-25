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
    // Ask the running application to redraw. Safe to call from any thread
    // (used by setters that change widget state outside the event loop).
    void requestRedraw();

    ftxui::Component component_;
};

}