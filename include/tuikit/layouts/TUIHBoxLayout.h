#pragma once
#include "tuikit/layouts/TUILayout.h"
#include <vector>
#include <ftxui/component/component.hpp>
namespace TUIKIT {
class TUIHBoxLayout : public TUILayout {
public:
    TUIHBoxLayout(TUIWidget* parent = nullptr);
    void addWidget(std::shared_ptr<TUIWidget> widget);
private:
    std::vector<std::shared_ptr<TUIWidget>> widgets_;
};
}