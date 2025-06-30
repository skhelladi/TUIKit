#pragma once
#include "tuikit/layouts/TUILayout.h"
#include <vector>
#include <string>
#include <functional>
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
namespace TUIKIT {
class TUIRadioBox : public TUILayout {
public:
    using OnSelectCallback = std::function<void(int selected_index)>;
    TUIRadioBox(const std::vector<std::string>& options, int initial_selected = 0, TUIWidget* parent = nullptr);
    void onSelect(OnSelectCallback cb);
    void setOptions(const std::vector<std::string>& options);
    int selectedIndex() const;
    std::string selectedText() const;
    void setSelectedIndex(int index);
    ftxui::Component get_ftxui_component() override;
private:
    std::vector<std::string> options_;
    int selected_index_ = 0;
    OnSelectCallback on_select_;
    ftxui::Component ftxui_radiobox_component_;
    void update_ftxui_radiobox_component();
};
}