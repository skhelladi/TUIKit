#pragma once
#include "tuikit/core/TUIWidget.h"
#include <string>
#include <functional>
#include <ftxui/component/component.hpp>
namespace TUIKIT {
class TUITextField : public TUIWidget {
public:
    using OnChangeCallback = std::function<void(const std::string&)>;
    using OnEnterCallback = std::function<void()>;

    TUITextField(const std::string& placeholder = "", TUIWidget* parent = nullptr);

    void onChange(OnChangeCallback cb);
    void onEnter(OnEnterCallback cb);

    std::string text() const;

private:
    std::string value_;
    OnChangeCallback on_change_;
    OnEnterCallback on_enter_;
};
}
