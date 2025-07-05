#pragma once
#include "tuikit/core/TUIWidget.h"
#include <string>
#include <functional>
#include <ftxui/component/component.hpp>
namespace TUIKIT {
class TUICheckBox : public TUIWidget {
public:
    using OnChangeCallback = std::function<void(bool)>;
    TUICheckBox(const std::string& text, bool initial_checked = false, TUIWidget* parent = nullptr);
    void onChange(OnChangeCallback cb);
    bool checked() const;
private:
    bool checked_ = false;
    OnChangeCallback on_change_;
    std::string text_;
};
}
