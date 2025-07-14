#pragma once

#include "tuikit/core/TUIWidget.h"
#include <string>
#include <vector>
#include <functional>

namespace TUIKIT {

class TUITableView : public TUIWidget {
public:
    using OnSelectCallback = std::function<void(int, int)>; // row, col
    using OnEnterCallback = std::function<void(int, int)>;  // row, col
    using OnEditCompleteCallback = std::function<void(int, int, const std::string&)>; // row, col, new_value

    TUITableView(const std::vector<std::string>& headers, const std::vector<std::vector<std::string>>& data, TUIWidget* parent = nullptr);

    void onSelect(OnSelectCallback cb);
    void onEnter(OnEnterCallback cb);
    void onEditComplete(OnEditCompleteCallback cb);

    void setReadOnly(bool read_only);
    bool isReadOnly() const;

private:
    std::vector<std::string> headers_;
    std::vector<std::vector<std::string>> data_;
    OnSelectCallback on_select_;
    OnEnterCallback on_enter_;
    OnEditCompleteCallback on_edit_complete_;
    int selected_row_ = 0;
    int selected_col_ = 0;
    bool is_editing_ = false;
    bool read_only_ = false;
    std::string editing_text_;
    ftxui::Component input_component_;
    ftxui::Component table_component_; // New member for the table component
    std::vector<int> column_widths_;
};

}