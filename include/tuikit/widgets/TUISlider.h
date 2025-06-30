#ifndef TUIKIT_TUISLIDER_H
#define TUIKIT_TUISLIDER_H

#include "tuikit/core/TUIWidget.h"
#include <string>
#include <functional>
#include <ftxui/component/component.hpp>

namespace TUIKIT {

class TUISlider : public TUIWidget {
public:
    using OnChangeCallback = std::function<void(float value)>;

    TUISlider(const std::string& label, float initial_value, float min_value, float max_value, float increment, TUIWidget* parent = nullptr);
    ~TUISlider() override = default;

    ftxui::Component get_ftxui_component() override;

    float value() const;
    void setValue(float new_value);
    void onChange(OnChangeCallback callback);

private:
    std::string label_;
    float value_;
    float min_value_;
    float max_value_;
    float increment_;
    OnChangeCallback on_change_callback_;
};

} // namespace TUIKIT

#endif // TUIKIT_TUISLIDER_H
