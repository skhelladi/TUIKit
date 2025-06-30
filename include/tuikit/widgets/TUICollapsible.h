#pragma once
#include "tuikit/core/TUIWidget.h"
#include <string>
#include <ftxui/component/component.hpp>
namespace TUIKIT {
class TUICollapsible : public TUIWidget {
public:
    TUICollapsible(const std::string& title, std::shared_ptr<TUIWidget> content, TUIWidget* parent = nullptr);
    ftxui::Component get_ftxui_component() override;
private:
    std::string title_;
    std::shared_ptr<TUIWidget> content_ = nullptr;
};
}
