#pragma once

#include "tuikit/core/TUIWidget.h"
#include <functional>

namespace TUIKIT
{

    class TUISpinBox : public TUIWidget
    {
    public:
        using OnChangeCallback = std::function<void(int)>;
        using OnEnterCallback = std::function<void()>;

        TUISpinBox(const std::string& label, int initial_value, int min = 0, int max = 100, int increment = 1, TUIWidget *parent = nullptr);

        void onIncrement(OnChangeCallback cb);
        void onDecrement(OnChangeCallback cb);

        void onChange(OnChangeCallback cb);
        void onEnter(OnEnterCallback cb);

        int value() const;
        void setValue(int value);

    private:
        std::string value_str_;
        int value_;
        int min_;
        int max_;
        OnChangeCallback on_change_;
        OnEnterCallback on_enter_;
    };

}