#include "tuikit/widgets/TUIDoubleSpinBox.h"
#include "ftxui/component/component.hpp"
#include "ftxui/component/event.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include <iomanip>
#include <sstream>

namespace TUIKIT
{

    TUIDoubleSpinBox::TUIDoubleSpinBox(const std::string& label, double initial_value, double min, double max, 
    double increment, TUIWidget* parent)
    : value_str_(std::to_string(initial_value)), TUIWidget(ftxui::Input(&value_str_, label)), value_(initial_value), min_(min), max_(max), on_change_(nullptr), on_enter_(nullptr)
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
                value_str_ = std::to_string(value_);
            }
        } catch (const std::exception& e) {
            value_str_ = std::to_string(value_);
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
    component_ = ftxui::CatchEvent(input_component, [this, increment](ftxui::Event event) {
        if (read_only_) return false;
        if (event == ftxui::Event::ArrowRight) {
            setValue(value_ + increment);
            value_str_ = std::to_string(value_);
            if (on_change_) {
                on_change_(value_);
            }
            return true;
        }
        if (event == ftxui::Event::ArrowLeft) {
            setValue(value_ - increment);
            value_str_ = std::to_string(value_);
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
}

    void TUIDoubleSpinBox::onChange(OnChangeCallback cb)
    {
        on_change_ = cb;
    }

    void TUIDoubleSpinBox::onEnter(OnEnterCallback cb)
    {
        on_enter_ = cb;
    }

    void TUIDoubleSpinBox::onIncrement(OnChangeCallback cb)
    {
        // Not directly used, handled internally by on_event
    }

    void TUIDoubleSpinBox::onDecrement(OnChangeCallback cb)
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
        }
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