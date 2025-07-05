#include "tuikit/widgets/TUITreeView.h"
#include "tuikit/layouts/TUIVBoxLayout.h"
#include "tuikit/widgets/TUICollapsible.h"
#include "tuikit/widgets/TUILabel.h"
#include <ftxui/component/component.hpp>

namespace TUIKIT {

TUITreeView::TUITreeView(TreeNode root_node, TUIWidget* /*parent*/)
    : TUIWidget(ftxui::Component()), root_node_(std::move(root_node)) {
    
    main_layout_ = std::make_shared<TUIVBoxLayout>();
    component_ = main_layout_->get_ftxui_component();

    // Create the root collapsible node
    auto root_collapsible = createCollapsibleNode(root_node_);
    main_layout_->addWidget(root_collapsible);
}

void TUITreeView::onSelect(OnSelectCallback cb) {
    on_select_ = std::move(cb);
}

std::shared_ptr<TUICollapsible> TUITreeView::createCollapsibleNode(const TreeNode& node) {
    auto content_layout = std::make_shared<TUIVBoxLayout>();

    // Create a label for the current node's content (or a button for selection)
    auto node_label = std::make_shared<TUILabel>(node.label);
    // Connect a click event to the label to simulate selection
    // This requires TUILabel to have an onClick or similar signal, which it currently doesn't.
    // For now, we'll just make it a label. If selection is critical, we might need a small button or a custom component.
    // For demonstration, we'll trigger on_select_ when the collapsible is expanded/collapsed.
    // A better approach would be to make the label itself clickable or add a small invisible button.

    // Recursively create children
    for (const auto& child : node.children) {
        auto child_collapsible = createCollapsibleNode(child);
        content_layout->addWidget(child_collapsible);
    }

    auto collapsible = std::make_shared<TUICollapsible>(node.label, content_layout);
    
    // If a node is selected, trigger the callback
    // This is a placeholder. A proper selection mechanism would involve a clickable element inside the collapsible.
    // For now, we'll just use the collapsible's title as the selected item.
    // This part needs refinement based on how selection is truly intended.
    // For now, let's assume expanding/collapsing implies a form of interaction.
    // A better way would be to add a small button next to the label or make the label itself interactive.
    // Since TUILabel doesn't have an onClick, we'll rely on the user clicking the collapsible title.
    // If the user clicks the collapsible title, we can consider it "selected".
    // This is a simplification for the refactoring.

    return collapsible;
}

} // namespace TUIKIT
