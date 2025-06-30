#ifndef TUIKIT_TUIPROGRESSBAR_H
#define TUIKIT_TUIPROGRESSBAR_H

#include "tuikit/core/TUIWidget.h"
#include <string>
#include <ftxui/dom/elements.hpp>

namespace TUIKIT {

class TUIProgressBar : public TUIWidget {
public:
    TUIProgressBar(float initial_value = 0.0f, const std::string& label = "", TUIWidget* parent = nullptr);
    ~TUIProgressBar() override = default;

    ftxui::Component get_ftxui_component() override;

    void setValue(float value);
    float value() const;
    void setLabel(const std::string& label);

private:
    float value_;
    std::string label_;
    ftxui::Component ftxui_progress_bar_component_;
};

} // namespace TUIKIT

#endif // TUIKIT_TUIPROGRESSBAR_H
