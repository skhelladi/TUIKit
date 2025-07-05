#ifndef TUIKIT_TUIMENU_H
#define TUIKIT_TUIMENU_H

#include "tuikit/core/TUIWidget.h"
#include <string>
#include <vector>
#include <functional>
#include <ftxui/component/component.hpp>

namespace TUIKIT {

class TUIMenu : public TUIWidget {
public:
    using OnSelectCallback = std::function<void(int selected_index)>;

    TUIMenu(const std::vector<std::string>& options, TUIWidget* parent = nullptr);
    ~TUIMenu() override = default;

    void setOptions(const std::vector<std::string>& options);
    void setIcons(const std::vector<std::string>& icons); // Associe une icône à chaque option
    void onSelect(OnSelectCallback callback);
    int selectedIndex() const;
    std::string selectedText() const;
    void setSelectedIndex(int index);

private:
    std::vector<std::string> options_;
    std::vector<std::string> icons_;
    std::vector<std::string> options_with_icons_cache_; // New member to store combined options
    int selected_index_ = 0;
    OnSelectCallback on_select_callback_;

    void update_ftxui_menu_component();
};

} // namespace TUIKIT

#endif // TUIKIT_TUIMENU_H
