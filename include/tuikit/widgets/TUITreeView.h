#pragma once

#include "tuikit/core/TUIWidget.h"
#include <string>
#include <vector>
#include <functional>
#include <memory>
#include "tuikit/layouts/TUIVBoxLayout.h"
#include "tuikit/widgets/TUICollapsible.h"
#include "tuikit/widgets/TUIButton.h"
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>

namespace TUIKIT {

struct TreeNode {
    std::string label;
    std::vector<TreeNode> children;
    bool is_expanded = false;
    ftxui::Component component;
};

class TUITreeView : public TUIWidget {
public:
    using OnSelectCallback = std::function<void(const std::string&)>;

    TUITreeView(TreeNode root_node);
    ~TUITreeView() override = default;

    void onSelect(OnSelectCallback cb);

private:
    void buildComponent(TreeNode& node, int depth);
    ftxui::Element renderNode(const TreeNode& node, int depth);

    TreeNode root_node_;
    OnSelectCallback on_select_;
};

} // namespace TUIKIT
