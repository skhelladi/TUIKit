#pragma once
#include "tuikit/layouts/TUILayout.h"
#include <vector>
#include <ftxui/component/component.hpp>
namespace TUIKIT {
class TUIVBoxLayout : public TUILayout {
public:
    TUIVBoxLayout(TUIWidget* parent = nullptr);
    void addWidget(std::shared_ptr<TUIWidget> widget);
private:
    std::vector<std::shared_ptr<TUIWidget>> widgets_;
};
}