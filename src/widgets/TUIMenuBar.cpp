#include "tuikit/widgets/TUIMenuBar.h"
#include "tuikit/core/TUIStyle.h"
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/dom/elements.hpp>

namespace TUIKIT {

using namespace ftxui; // NOLINT

namespace {

bool selectable(const MenuItem& item) {
    return !item.separator && item.enabled;
}

} // namespace

TUIMenuBar::TUIMenuBar(TUIWidget* /*parent*/)
    : TUIWidget(Component()) {
    titles_container_ = Container::Horizontal({});
    dropdown_container_ = Container::Vertical({});

    auto content = Container::Vertical({titles_container_, dropdown_container_});
    component_ = Renderer(content, [this] { return render(); });
    component_ = CatchEvent(component_, [this](Event event) { return handleEvent(event); });
}

void TUIMenuBar::addMenu(const std::string& title, const std::vector<MenuItem>& items) {
    const int menu_index = static_cast<int>(menus_.size());
    menus_.push_back({title, items});

    ButtonOption option;
    option.transform = [this, menu_index, title](const EntryState& state) {
        auto element = text(" " + title + " ");
        if (state.focused || open_menu_ == menu_index) {
            element |= inverted;
        }
        return element;
    };
    auto title_button = Button(
        title,
        [this, menu_index] {
            if (open_menu_ == menu_index) {
                closeMenu();
            } else {
                openMenu(menu_index);
            }
            requestRedraw();
        },
        option);
    titles_container_->Add(title_button);
}

void TUIMenuBar::closeMenu() {
    open_menu_ = -1;
    item_index_ = -1;
    dropdown_container_->DetachAllChildren();
}

bool TUIMenuBar::isMenuOpen() const {
    return open_menu_ >= 0;
}

void TUIMenuBar::openMenu(int index) {
    if (index < 0 || index >= static_cast<int>(menus_.size())) {
        closeMenu();
        return;
    }
    open_menu_ = index;
    item_index_ = -1;
    rebuildDropdown();
    // Select the first selectable entry.
    moveItemSelection(1);
}

void TUIMenuBar::rebuildDropdown() {
    dropdown_container_->DetachAllChildren();
    if (open_menu_ < 0 || open_menu_ >= static_cast<int>(menus_.size())) {
        return;
    }
    const auto& items = menus_[open_menu_].items;

    std::size_t label_width = 0;
    for (const auto& item : items) {
        if (!item.separator) {
            const std::string label = (item.icon.empty() ? "" : item.icon + " ") + item.label;
            label_width = std::max(label_width, label.size());
        }
    }

    for (std::size_t k = 0; k < items.size(); ++k) {
        const MenuItem& item = items[k];
        if (item.separator) {
            dropdown_container_->Add(Renderer([] { return separator(); }));
            continue;
        }
        const std::string label = (item.icon.empty() ? "" : item.icon + " ") + item.label;
        const std::string shortcut = item.shortcut;
        const bool enabled = item.enabled;

        ButtonOption option;
        option.transform = [this, k, label, shortcut, enabled, label_width](const EntryState& state) {
            auto row = hbox({
                text(label + std::string(label_width - label.size(), ' ')),
                text("  "),
                text(shortcut) | dim,
            });
            if (state.focused || item_index_ == static_cast<int>(k)) {
                row |= inverted;
            }
            if (!enabled) {
                row |= dim;
            }
            return row | size(WIDTH, GREATER_THAN, static_cast<int>(label_width + 4));
        };
        auto item_button = Button(
            label,
            [this, k] { activateItem(open_menu_, static_cast<int>(k)); },
            option);
        dropdown_container_->Add(item_button);
    }
}

void TUIMenuBar::activateItem(int menu_index, int item_index) {
    if (menu_index < 0 || menu_index >= static_cast<int>(menus_.size())) {
        return;
    }
    const auto& items = menus_[menu_index].items;
    if (item_index < 0 || item_index >= static_cast<int>(items.size())) {
        return;
    }
    if (!selectable(items[item_index])) {
        return;
    }
    auto action = items[item_index].action;
    closeMenu();
    if (action) {
        action();
    }
    requestRedraw();
}

void TUIMenuBar::moveItemSelection(int delta) {
    if (open_menu_ < 0 || open_menu_ >= static_cast<int>(menus_.size())) {
        return;
    }
    const auto& items = menus_[open_menu_].items;
    const int count = static_cast<int>(items.size());
    if (count == 0) {
        return;
    }
    for (int step = 0; step < count; ++step) {
        item_index_ = (item_index_ + delta + count) % count;
        if (selectable(items[item_index_])) {
            return;
        }
    }
}

bool TUIMenuBar::handleEvent(Event event) {
    // Let mouse events reach the title and item buttons untouched.
    if (event.is_mouse()) {
        return false;
    }
    if (open_menu_ < 0) {
        return false; // Title buttons handle keyboard when nothing is open.
    }
    const int menu_count = static_cast<int>(menus_.size());

    if (event == Event::Escape) {
        closeMenu();
        requestRedraw();
        return true;
    }
    if (event == Event::ArrowLeft) {
        openMenu((open_menu_ - 1 + menu_count) % menu_count);
        requestRedraw();
        return true;
    }
    if (event == Event::ArrowRight) {
        openMenu((open_menu_ + 1) % menu_count);
        requestRedraw();
        return true;
    }
    if (event == Event::ArrowDown) {
        moveItemSelection(1);
        requestRedraw();
        return true;
    }
    if (event == Event::ArrowUp) {
        moveItemSelection(-1);
        requestRedraw();
        return true;
    }
    if (event == Event::Return || event == Event::Character(" ")) {
        activateItem(open_menu_, item_index_);
        return true;
    }
    return false;
}

Element TUIMenuBar::render() {
    auto& theme = TUIStyle::instance().currentTheme();
    Element bar = titles_container_->Render();
    if (open_menu_ < 0 || open_menu_ >= static_cast<int>(menus_.size())) {
        return bar;
    }

    // Horizontal offset of the open menu title, so the dropdown lines up
    // underneath it (each title renders as " title ", i.e. len + 2 cells).
    std::size_t offset = 0;
    for (int i = 0; i < open_menu_; ++i) {
        offset += menus_[i].title.size() + 2;
    }

    Element dropdown = dropdown_container_->Render();
    dropdown = std::move(dropdown) | border | color(theme.text) | bgcolor(theme.background);
    return vbox({
        bar,
        hbox({text(std::string(offset, ' ')), dropdown}),
    });
}

} // namespace TUIKIT
