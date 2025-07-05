#include "tuikit/widgets/TUITreeView.h"
#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"
#include "ftxui/component/component_options.hpp"

namespace TUIKIT {

TUITreeView::TUITreeView(TreeNode root_node)
    : TUIWidget(ftxui::Component()), root_node_(std::move(root_node)) {
    buildComponent(root_node_, 0);
    component_ = root_node_.component;
}

void TUITreeView::onSelect(OnSelectCallback cb) {
    on_select_ = std::move(cb);
}

void TUITreeView::buildComponent(TreeNode& node, int depth) {
    using namespace ftxui;

    if (node.children.empty()) {
        auto option = ButtonOption();
        option.transform = [=, &node](const EntryState& state) {
            std::string indentation = std::string(depth * 3, ' ');
            auto element = hbox({
                text(indentation + "  " + node.label)
            });
            if (state.focused) {
                element = element | inverted;
            }
            return element;
        };
        node.component = Button(&node.label, [this, &node] {
            if (on_select_) {
                on_select_(node.label);
            }
        }, option);
    } else {
        std::vector<Component> children_components;
        for (auto& child : node.children) {
            buildComponent(child, depth + 1);
            children_components.push_back(child.component);
        }
        auto children_container = Container::Vertical(children_components);
        
        auto option = ButtonOption();
        option.transform = [=, &node](const EntryState& state) {
            std::string indentation = std::string(depth * 3, ' ');
            std::string icon = node.is_expanded ? "▼ " : "► ";
            auto element = hbox({
                text(indentation + icon + node.label)
            });
            if (state.focused) {
                element = element | inverted;
            }
            return element;
        };
        auto header_button = Button(&node.label, [&node] { node.is_expanded = !node.is_expanded; }, option);
        
        auto combined = Container::Vertical({header_button, Maybe(children_container, &node.is_expanded)});

        node.component = combined;
    }
}

} // namespace TUIKIT
