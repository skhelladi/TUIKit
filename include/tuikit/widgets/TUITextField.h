#pragma once
#include "tuikit/core/TUIWidget.h"
#include <string>
#include <functional>
#include <ftxui/component/component.hpp>
namespace TUIKIT {
class TUITextField : public TUIWidget {
public:
    using OnChangeCallback = std::function<void(const std::string&)>;
    TUITextField(const std::string& placeholder = "", TUIWidget* parent = nullptr);
    void onChange(OnChangeCallback cb);
    std::string text() const;
private:
    std::string value_;
    OnChangeCallback on_change_;
};
}
