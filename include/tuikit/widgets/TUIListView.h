#pragma once

#include "tuikit/core/TUIWidget.h"
#include <string>
#include <vector>
#include <functional>

namespace TUIKIT {

class TUIListView : public TUIWidget {
public:
    using OnSelectCallback = std::function<void(int)>;
    using OnEnterCallback = std::function<void(int)>;

    TUIListView(const std::vector<std::string>& items, TUIWidget* parent = nullptr);

    void onSelect(OnSelectCallback cb);
    void onEnter(OnEnterCallback cb);

    int selectedIndex() const;
    void setSelectedIndex(int index);
    std::string selectedText() const;

    void setItems(const std::vector<std::string>& items);

private:
    void update_ftxui_menu_component();

private:
    std::vector<std::string> items_;
    int selected_index_ = 0;
    OnSelectCallback on_select_;
    OnEnterCallback on_enter_;
};

}