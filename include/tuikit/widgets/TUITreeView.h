#pragma once

#include "tuikit/core/TUIWidget.h"
#include <string>
#include <vector>
#include <functional>
#include <memory>
#include "tuikit/layouts/TUIVBoxLayout.h"
#include "tuikit/widgets/TUICollapsible.h"
#include "tuikit/widgets/TUIButton.h" // Added for clickable nodes

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

    void onSelect(OnSelectCallback cb);

    std::shared_ptr<TUICollapsible> createCollapsibleNode(const TreeNode &node);

private:
    TreeNode root_node_;
    OnSelectCallback on_select_;

    std::shared_ptr<class TUIVBoxLayout> main_layout_;

};

} // namespace TUIKIT
