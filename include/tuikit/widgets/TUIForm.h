#pragma once
#include "tuikit/core/TUIWidget.h"
#include <vector>
#include <string>
#include <ftxui/component/component.hpp>
namespace TUIKIT {
class TUIForm : public TUIWidget {
public:
    TUIForm(TUIWidget* parent = nullptr);
    void addField(const std::string& label, std::shared_ptr<TUIWidget> field);
private:
    std::vector<std::pair<std::string, std::shared_ptr<TUIWidget>>> fields_;
};
}
