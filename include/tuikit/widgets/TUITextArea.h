#ifndef TUIKIT_TUITEXTAREA_H
#define TUIKIT_TUITEXTAREA_H

#include "tuikit/core/TUIWidget.h"
#include <string>
#include <functional>
#include <ftxui/component/component.hpp>

namespace TUIKIT {

class TUITextArea : public TUIWidget {
public:
    using OnChangeCallback = std::function<void(const std::string&)>;

    TUITextArea(const std::string& placeholder = "", const std::string& label = "", int height = 5, TUIWidget* parent = nullptr);
    ~TUITextArea() override = default;

    void onChange(OnChangeCallback cb);
    std::string text() const;
    void setText(const std::string& text);
    void setReadOnly(bool read_only);
    bool isReadOnly() const;

private:
    std::string value_;
    std::string placeholder_;
    std::string label_;
    int height_;
    bool read_only_ = false;
    OnChangeCallback on_change_;
    ftxui::Component input_ftxui_component_;
    ftxui::InputOption input_options_;
};

} // namespace TUIKIT

#endif // TUIKIT_TUITEXTAREA_H
