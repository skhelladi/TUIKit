#pragma once
#include "tuikit/core/TUIWidget.h"
#include <string>
#include <ftxui/component/component.hpp>
namespace TUIKIT {
class TUIGroupBox : public TUIWidget {
public:
    TUIGroupBox(const std::string& title, std::shared_ptr<TUIWidget> content_widget, bool show_border = true, TUIWidget* parent = nullptr);
    ftxui::Component get_ftxui_component() override;
    void setTitle(const std::string& title);
    void setContentWidget(std::shared_ptr<TUIWidget> content_widget);
    void setShowBorder(bool show_border);
private:
    std::string title_;
    std::shared_ptr<TUIWidget> content_widget_ = nullptr;
    bool show_border_ = true;
};
}
