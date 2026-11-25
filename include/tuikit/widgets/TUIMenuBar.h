#ifndef TUIKIT_TUIMENUBAR_H
#define TUIKIT_TUIMENUBAR_H

#include "tuikit/core/TUIWidget.h"
#include <functional>
#include <string>
#include <vector>

namespace TUIKIT {

/// One entry inside a dropdown menu. Set `separator = true` to render a
/// horizontal separator line instead of a labelled entry (then only `enabled`
/// is meaningful among the other fields).
struct MenuItem {
    std::string label;
    std::function<void()> action;
    std::string icon;        // Optional, prepended to the label.
    bool separator = false;
    bool enabled = true;
    std::string shortcut;    // Optional, displayed right-aligned (e.g. "Ctrl+O").
};

/// Horizontal menu bar (File, Edit, View, ...) with keyboard-navigable
/// dropdowns. Typical usage: place it at the top of a vertical layout:
///
///     vbox({menubar_widget, content});
///
/// Keyboard: Left/Right switch menus, Up/Down navigate entries,
/// Enter/Space activate, Escape closes. Shortcuts are display-only.
class TUIMenuBar : public TUIWidget {
public:
    TUIMenuBar(TUIWidget* parent = nullptr);
    ~TUIMenuBar() override = default;

    /// Append a dropdown menu with the given title and entries.
    void addMenu(const std::string& title, const std::vector<MenuItem>& items);

    void closeMenu();
    bool isMenuOpen() const;

private:
    struct Menu {
        std::string title;
        std::vector<MenuItem> items;
    };

    void openMenu(int index);
    void rebuildDropdown();
    void activateItem(int menu_index, int item_index);
    void moveItemSelection(int delta);
    bool handleEvent(ftxui::Event event);
    ftxui::Element render();

    std::vector<Menu> menus_;
    ftxui::Component titles_container_;
    ftxui::Component dropdown_container_;
    int open_menu_ = -1;
    int item_index_ = -1;
};

} // namespace TUIKIT

#endif // TUIKIT_TUIMENUBAR_H
