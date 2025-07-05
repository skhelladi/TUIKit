#ifndef TUIKIT_TUITREEVIEW_H
#define TUIKIT_TUITREEVIEW_H

#include "tuikit/core/TUIWidget.h"
#include <string>
#include <vector>
#include <ftxui/component/component.hpp>

namespace TUIKIT {

struct TreeNode {
    std::string label;
    std::vector<TreeNode> children;
    bool is_expanded = false;
};

class TUITreeView : public TUIWidget {
public:
    using OnSelectCallback = std::function<void(const std::string&)>;

    TUITreeView(TreeNode root_node, TUIWidget* parent = nullptr);
    ~TUITreeView() override = default;

    ftxui::Component get_ftxui_component() override;
    void onSelect(OnSelectCallback cb);

private:
    TreeNode root_node_;
    ftxui::Component ftxui_tree_view_component_;
    OnSelectCallback on_select_;

    ftxui::Component build_ftxui_tree_component(TreeNode& node);
};

} // namespace TUIKIT

#endif // TUIKIT_TUITREEVIEW_H
