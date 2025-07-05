#ifndef TUIKIT_TUIRESIZABLESPLIT_H
#define TUIKIT_TUIRESIZABLESPLIT_H

#include "tuikit/core/TUIWidget.h"
#include <ftxui/component/component.hpp> // Use component.hpp for resizable_split

namespace TUIKIT {

class TUIResizableSplit : public TUIWidget {
public:
    enum Orientation {
        Horizontal, // Splits horizontally, widgets are side-by-side
        Vertical    // Splits vertically, widgets are top-and-bottom
    };

    TUIResizableSplit(std::shared_ptr<TUIWidget> first_widget, std::shared_ptr<TUIWidget> second_widget, Orientation orientation = Horizontal, TUIWidget* parent = nullptr);
    ~TUIResizableSplit() override = default;

    void setMinimumSizes(int min_first, int min_second);

private:
    std::shared_ptr<TUIWidget> first_widget_;
    std::shared_ptr<TUIWidget> second_widget_;
    Orientation orientation_;
    int main_size_ = 0; // To store the size of the main panel for resizable split
    int min_first_ = 0;
    int min_second_ = 0;
};

} // namespace TUIKIT

#endif // TUIKIT_TUIRESIZABLESPLIT_H
