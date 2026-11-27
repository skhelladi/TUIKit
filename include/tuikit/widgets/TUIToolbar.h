#ifndef TUIKIT_TUITOOLBAR_H
#define TUIKIT_TUITOOLBAR_H

#include "tuikit/core/TUIWidget.h"
#include "tuikit/layouts/TUIHBoxLayout.h"
#include "tuikit/widgets/TUIButton.h"
#include <string>
#include <vector>
#include <functional>
#include <ftxui/component/component.hpp>

namespace TUIKIT {

class TUIToolbar : public TUIWidget {
public:
    TUIToolbar(TUIWidget* parent = nullptr);
    ~TUIToolbar() override = default;

    std::shared_ptr<TUIButton> addButton(const std::string& text, TUIButton::OnClickedCallback on_clicked = nullptr, const std::string& icon = "");

private:
    std::shared_ptr<TUIHBoxLayout> toolbar_layout_ = nullptr;
    int item_count_ = 0; // number of buttons added, to place separators
};

} // namespace TUIKIT

#endif // TUIKIT_TUITOOLBAR_H
