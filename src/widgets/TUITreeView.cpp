#include "tuikit/widgets/TUITreeView.h"
#include <ftxui/component/component.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/dom/elements.hpp>
#include <iostream> // For debug output

namespace TUIKIT {

TUITreeView::TUITreeView(TreeNode root_node, TUIWidget* parent)
    : TUIWidget(parent), root_node_(std::move(root_node)) {
    ftxui_tree_view_component_ = build_ftxui_tree_component(root_node_);
}

void TUITreeView::onSelect(OnSelectCallback cb) {
    on_select_ = cb;
    ftxui_tree_view_component_ = build_ftxui_tree_component(root_node_);
}

ftxui::Component TUITreeView::get_ftxui_component() {
    return ftxui_tree_view_component_;
}

ftxui::Component TUITreeView::build_ftxui_tree_component(TreeNode& node) {
    // Create a component for the current node's label
    auto node_label_component = ftxui::Renderer([&node] {
        ftxui::Element label = ftxui::text(node.label);
        if (!node.children.empty()) {
            label = ftxui::hbox({(node.is_expanded ? ftxui::text("▼ ") : ftxui::text("► ")) | ftxui::color(ftxui::Color::Blue) | ftxui::bold, label});
        }
        return label;
    });

    // Event handler for the current node's label
    auto node_event_handler = [&](ftxui::Event event) {
        std::cerr << "TUITreeView Node Event for " << node.label << ": Event received." << std::endl; // Debug output
        if (event.is_mouse() && event.mouse().button == ftxui::Mouse::Left && event.mouse().motion == ftxui::Mouse::Pressed) {
            if (on_select_) {
                on_select_(node.label);
            }

            if (!node.children.empty()) {
                node.is_expanded = !node.is_expanded;
            }
            return true;
        } else if (event == ftxui::Event::Return || event == ftxui::Event::Character(' ')) {
            if (on_select_) {
                on_select_(node.label);
            }
            if (!node.children.empty()) {
                node.is_expanded = !node.is_expanded;
            }
            return true;
        }
        return false;
    };

    // Apply event handler to the node's label component
    ftxui::Component interactive_node_component = ftxui::CatchEvent(node_label_component, node_event_handler);

    // Recursively build components for children
    std::vector<ftxui::Component> children_components_list;
    if (node.is_expanded) {
        for (auto& child : node.children) {
            // Each child is a full component, not just its rendered element
            children_components_list.push_back(build_ftxui_tree_component(child));
        }
    }

    // Combine the current node's component with its children's components
    std::vector<ftxui::Component> components_to_render;
    components_to_render.push_back(interactive_node_component);
    for (const auto& child_comp : children_components_list) {
        // Create a new component for the indented child
        auto indentation_component = ftxui::Renderer([]{ return ftxui::text("  "); });
        auto indented_child_component = ftxui::Container::Horizontal({
            indentation_component, // Indentation as a component
            child_comp // The actual child component
        });
        components_to_render.push_back(indented_child_component);
    }

    return ftxui::Container::Vertical(components_to_render);
}

} // namespace TUIKIT
