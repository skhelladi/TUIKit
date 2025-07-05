#ifndef TUIKIT_TUILABEL_H
#define TUIKIT_TUILABEL_H

#include "tuikit/core/TUIWidget.h"
#include <string>
#include <ftxui/dom/elements.hpp>

namespace TUIKIT {

class TUILabel : public TUIWidget {
public:
    TUILabel(const std::string& text = "", TUIWidget* parent = nullptr);
    ~TUILabel() override = default;

    void setText(const std::string& text);
    std::string text() const;

private:
    std::string text_;
};

} // namespace TUIKIT

#endif // TUIKIT_TUILABEL_H
