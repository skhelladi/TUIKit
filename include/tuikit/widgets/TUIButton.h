#ifndef TUIKIT_TUIBUTTON_H
#define TUIKIT_TUIBUTTON_H

#include "tuikit/core/TUIWidget.h"
#include <string>
#include <functional>
#include <ftxui/component/component.hpp>

namespace TUIKIT {

class TUIButton : public TUIWidget {
public:
    using OnClickedCallback = std::function<void()>;

    TUIButton(const std::string& text, TUIWidget* parent = nullptr);
    ~TUIButton() override = default;

    ftxui::Component get_ftxui_component() override;
    void onClick(OnClickedCallback callback); // Setter for the click callback
    void setIcon(const std::string& icon);
    const std::string& icon() const;

private:
    std::string text_; // This will hold the text displayed by the FTXUI button (icon + original_text_)
    std::string original_text_; // This will hold the text provided by the user, without the icon
    std::string icon_;
    OnClickedCallback on_clicked_;
    ftxui::Component ftxui_button_component_;
};

} // namespace TUIKIT

#endif // TUIKIT_TUIBUTTON_H
