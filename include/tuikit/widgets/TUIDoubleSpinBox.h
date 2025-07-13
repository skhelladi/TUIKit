#pragma once

#include "tuikit/core/TUIWidget.h"
#include <functional>

namespace TUIKIT {

class TUIDoubleSpinBox : public TUIWidget {
public:
    using OnChangeCallback = std::function<void(double)>;
    using OnEnterCallback = std::function<void()>;

    TUIDoubleSpinBox(const std::string& label, double initial_value, double min = 0.0, double max = 100.0, double increment = 1.0, TUIWidget* parent = nullptr);

    void onChange(OnChangeCallback cb);
    void onEnter(OnEnterCallback cb);
    void onIncrement(OnChangeCallback cb);
    void onDecrement(OnChangeCallback cb);

    double value() const;
    void setValue(double value);

private:
    std::string value_str_;
    double value_;
    double min_;
    double max_;
    OnChangeCallback on_change_;
    OnEnterCallback on_enter_;
};

}