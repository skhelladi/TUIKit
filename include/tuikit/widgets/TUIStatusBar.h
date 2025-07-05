#pragma once
#include "tuikit/core/TUIWidget.h"
#include <string>
#include <ftxui/component/component.hpp>
namespace TUIKIT {
class TUIStatusBar : public TUIWidget {
public:
    TUIStatusBar(const std::string& message = "", TUIWidget* parent = nullptr);
    void setMessage(const std::string& message);
    std::string message() const;
private:
    class TUILabel* message_label_ = nullptr;
};
}
