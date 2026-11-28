#include "tuikit/widgets/TUIDoubleSpinBox.h"
#include "tuikit/core/TUIStyle.h"
#include "ftxui/component/component.hpp"
#include "ftxui/component/event.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include <cmath>
#include <cstdio>

namespace TUIKIT
{
namespace {
// The display form of a double: %.10g keeps the value the user typed
// (1.716e-05 stays 1.716e-05 where std::to_string printed 0.000017) and
// drops the trailing zeros of the fixed six-decimal form.
std::string formatDouble(double value) {
    char buffer[64];
    std::snprintf(buffer, sizeof(buffer), "%.10g", value);
    return buffer;
}
} // namespace


    TUIDoubleSpinBox::TUIDoubleSpinBox(const std::string& label, double initial_value, double min, double max, 
    double increment, TUIWidget* /*parent*/)
    : TUIWidget(ftxui::Input(&value_str_, label)), value_str_(formatDouble(initial_value)), value_(initial_value), min_(min), max_(max), on_change_(nullptr), on_enter_(nullptr)
{
    ftxui::InputOption option;
    option.on_change = [this] {
        if (read_only_) return;
        try {
            double new_value = std::stod(value_str_);
            if (new_value >= min_ && new_value <= max_) {
                value_ = new_value;
                if (on_change_) {
                    on_change_(value_);
                }
            } else {
                value_str_ = formatDouble(value_);
            }
        } catch (const std::exception& e) {
            value_str_ = formatDouble(value_);
        }
    };

    option.on_enter = [this] {
        if (read_only_) return;
        if (on_enter_) {
            on_enter_();
        }
    };

    auto input_component = ftxui::Input(&value_str_, label, option);

    // Wrap the input_component to handle ArrowUp/ArrowDown events
    auto event_handler = ftxui::CatchEvent(input_component, [this, increment](ftxui::Event event) {
        if (read_only_) return false;
        if (event == ftxui::Event::ArrowRight) {
            setValue(value_ + increment);
            value_str_ = formatDouble(value_);
            if (on_change_) {
                on_change_(value_);
            }
            return true;
        }
        if (event == ftxui::Event::ArrowLeft) {
            setValue(value_ - increment);
            value_str_ = formatDouble(value_);
            if (on_change_) {
                on_change_(value_);
            }
            return true;
        }
        if (event == ftxui::Event::Escape) {
            return false; // Allow escape to propagate for navigation
        }
        return false;
    });

    component_ = TUIStyle::decorateField(event_handler);
}

    void TUIDoubleSpinBox::onChange(OnChangeCallback cb)
    {
        on_change_ = cb;
    }

    void TUIDoubleSpinBox::onEnter(OnEnterCallback cb)
    {
        on_enter_ = cb;
    }

    void TUIDoubleSpinBox::onIncrement(OnChangeCallback /*cb*/)
    {
        // Not directly used, handled internally by on_event
    }

    void TUIDoubleSpinBox::onDecrement(OnChangeCallback /*cb*/)
    {
        // Not directly used, handled internally by on_event
    }

    double TUIDoubleSpinBox::value() const
    {
        return value_;
    }

    void TUIDoubleSpinBox::setValue(double value)
    {
        if (value >= min_ && value <= max_)
        {
            value_ = value;
            value_str_ = formatDouble(value_);
            requestRedraw();
        }
    }

    void TUIDoubleSpinBox::setRange(double min, double max)
    {
        min_ = min;
        max_ = max;
        if (value_ < min_) setValue(min);
        else if (value_ > max_) setValue(max);
    }

    void TUIDoubleSpinBox::setReadOnly(bool read_only)
    {
        read_only_ = read_only;
    }

    bool TUIDoubleSpinBox::isReadOnly() const
    {
        return read_only_;
    }
}