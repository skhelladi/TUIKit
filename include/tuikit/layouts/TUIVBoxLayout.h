#pragma once
#include "tuikit/layouts/TUILayout.h"
#include <vector>
#include <ftxui/component/component.hpp>
namespace TUIKIT {
class TUIVBoxLayout : public TUILayout {
public:
    TUIVBoxLayout(TUIWidget* parent = nullptr);
    void addWidget(std::shared_ptr<TUIWidget> widget);
    ftxui::Component get_ftxui_component() override;
private:
    std::vector<std::shared_ptr<TUIWidget>> widgets_;
};
}
