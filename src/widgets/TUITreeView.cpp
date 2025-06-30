
#include "tuikit/widgets/TUITreeView.h"
#include <ftxui/component/component.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/dom/elements.hpp>

namespace TUIKIT {

TUITreeView::TUITreeView(TreeNode root_node, TUIWidget* parent)
    : TUIWidget(parent), root_node_(std::move(root_node)) {
    ftxui_tree_view_component_ = build_ftxui_tree_component(root_node_);
}

ftxui::Component TUITreeView::get_ftxui_component() {
    return ftxui_tree_view_component_;
}

ftxui::Component TUITreeView::build_ftxui_tree_component(TreeNode& node) {
    std::vector<ftxui::Component> children_components;
    for (auto& child : node.children) {
        children_components.push_back(build_ftxui_tree_component(child));
    }

    auto renderer = ftxui::Renderer([&node, children_components] {
        ftxui::Element label = ftxui::text(node.label);
        if (!node.children.empty()) {
            label = ftxui::hbox({(node.is_expanded ? ftxui::text("▼ ") : ftxui::text("► ")) | ftxui::color(ftxui::Color::Blue) | ftxui::bold, label});
        }

        if (node.is_expanded) {
            ftxui::Elements elements;
            elements.push_back(label);
            for (const auto& child_comp : children_components) {
                elements.push_back(ftxui::hbox({ftxui::text("  "), child_comp->Render()}));
            }
            return ftxui::vbox(elements);
        } else {
            return label;
        }
    });
    auto event_handler = [&, renderer](ftxui::Event event) {
        if (event.is_mouse() && event.mouse().button == ftxui::Mouse::Left && event.mouse().motion == ftxui::Mouse::Pressed) {
            if (!node.children.empty()) {
                node.is_expanded = !node.is_expanded;
                return true;
            }
        }
        return false;
    };
    return ftxui::CatchEvent(renderer, event_handler);
}
}

