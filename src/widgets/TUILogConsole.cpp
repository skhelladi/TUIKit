#include "tuikit/widgets/TUILogConsole.h"
#include "tuikit/core/TUIStyle.h"
#include <algorithm>
#include <cctype>
#include <ftxui/component/component.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/component/mouse.hpp>
#include <ftxui/dom/elements.hpp>

namespace TUIKIT {

using namespace ftxui; // NOLINT

namespace {

std::string toLower(const std::string& input) {
    std::string result = input;
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return result;
}

} // namespace

TUILogConsole::TUILogConsole(std::size_t capacity, TUIWidget* /*parent*/)
    : TUIWidget(Component()), capacity_(capacity) {
    // Focusable renderer: takes focus on click, so keyboard scrolling works.
    auto view = Renderer([this](bool) { return render(); });
    component_ = CatchEvent(view, [this](Event event) { return handleEvent(event); });
}

void TUILogConsole::append(const std::string& line) {
    appendLine({line, Color::Default});
}

void TUILogConsole::append(const std::string& line, Color color) {
    appendLine({line, color});
}

void TUILogConsole::appendLines(const std::vector<std::string>& lines) {
    std::lock_guard<std::mutex> lock(mutex_);
    for (const auto& line : lines) {
        lines_.push_back({line, Color::Default});
    }
    while (lines_.size() > capacity_) {
        lines_.pop_front();
    }
    filtered_dirty_ = true;
    requestRedraw();
}

void TUILogConsole::appendLines(const std::vector<Line>& lines) {
    std::lock_guard<std::mutex> lock(mutex_);
    for (const auto& line : lines) {
        lines_.push_back(line);
    }
    while (lines_.size() > capacity_) {
        lines_.pop_front();
    }
    filtered_dirty_ = true;
    requestRedraw();
}

void TUILogConsole::appendLine(Line line) {
    std::lock_guard<std::mutex> lock(mutex_);
    lines_.push_back(std::move(line));
    while (lines_.size() > capacity_) {
        lines_.pop_front();
    }
    filtered_dirty_ = true;
    requestRedraw();
}

void TUILogConsole::clear() {
    std::lock_guard<std::mutex> lock(mutex_);
    lines_.clear();
    filtered_.clear();
    filtered_dirty_ = false;
    scroll_offset_ = 0;
    follow_ = true;
    requestRedraw();
}

void TUILogConsole::setFilter(const std::string& filter) {
    std::lock_guard<std::mutex> lock(mutex_);
    filter_ = filter;
    filtered_dirty_ = true;
    requestRedraw();
}

std::string TUILogConsole::filter() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return filter_;
}

void TUILogConsole::setFollow(bool follow) {
    std::lock_guard<std::mutex> lock(mutex_);
    follow_ = follow;
    if (follow_) {
        scroll_offset_ = 0;
    }
    requestRedraw();
}

bool TUILogConsole::following() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return follow_;
}

void TUILogConsole::setCapacity(std::size_t capacity) {
    std::lock_guard<std::mutex> lock(mutex_);
    capacity_ = std::max<std::size_t>(1, capacity);
    while (lines_.size() > capacity_) {
        lines_.pop_front();
    }
    filtered_dirty_ = true;
}

std::size_t TUILogConsole::capacity() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return capacity_;
}

std::size_t TUILogConsole::lineCount() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return lines_.size();
}

void TUILogConsole::rebuildFiltered() const {
    filtered_.clear();
    if (filter_.empty()) {
        filtered_.reserve(lines_.size());
        for (std::size_t i = 0; i < lines_.size(); ++i) {
            filtered_.push_back(i);
        }
    } else {
        const std::string needle = toLower(filter_);
        for (std::size_t i = 0; i < lines_.size(); ++i) {
            if (toLower(lines_[i].text).find(needle) != std::string::npos) {
                filtered_.push_back(i);
            }
        }
    }
    filtered_dirty_ = false;
}

void TUILogConsole::scrollBy(int delta) {
    const int total = static_cast<int>(filtered_.size());
    if (delta > 0) {
        // Scrolling towards older lines disengages follow-tail.
        follow_ = false;
        scroll_offset_ = std::min(total - 1, scroll_offset_ + delta);
    } else {
        scroll_offset_ = std::max(0, scroll_offset_ + delta);
        if (scroll_offset_ == 0) {
            follow_ = true; // Back at the bottom: re-engage tail mode.
        }
    }
}

bool TUILogConsole::handleEvent(Event event) {
    if (event.is_mouse()) {
        const auto& mouse = event.mouse();
        // Only react when the cursor is over the console.
        if (!box_.Contain(mouse.x, mouse.y)) {
            return false;
        }
        if (mouse.button == Mouse::WheelUp) {
            std::lock_guard<std::mutex> lock(mutex_);
            if (filtered_dirty_) rebuildFiltered();
            scrollBy(3);
            requestRedraw();
            return true;
        }
        if (mouse.button == Mouse::WheelDown) {
            std::lock_guard<std::mutex> lock(mutex_);
            if (filtered_dirty_) rebuildFiltered();
            scrollBy(-3);
            requestRedraw();
            return true;
        }
        return false;
    }

    std::lock_guard<std::mutex> lock(mutex_);
    if (filtered_dirty_) rebuildFiltered();

    if (event == Event::ArrowUp) {
        scrollBy(1);
    } else if (event == Event::ArrowDown) {
        scrollBy(-1);
    } else if (event == Event::PageUp) {
        scrollBy(std::max(1, visible_height_));
    } else if (event == Event::PageDown) {
        scrollBy(-std::max(1, visible_height_));
    } else if (event == Event::Home) {
        follow_ = false;
        scroll_offset_ = std::max(0, static_cast<int>(filtered_.size()) - 1);
    } else if (event == Event::End) {
        follow_ = true;
        scroll_offset_ = 0;
    } else {
        return false;
    }
    requestRedraw();
    return true;
}

Element TUILogConsole::render() {
    std::lock_guard<std::mutex> lock(mutex_);
    if (filtered_dirty_) rebuildFiltered();

    // Height learned from the previous frame (filled by | reflect(box_)).
    visible_height_ = std::max(1, box_.y_max - box_.y_min + 1);

    const int total = static_cast<int>(filtered_.size());
    auto& theme = TUIStyle::instance().currentTheme();

    Elements row_elements;
    if (total > 0) {
        const int height = std::max(1, visible_height_);
        int last = follow_ ? (total - 1) : (total - 1 - scroll_offset_);
        last = std::clamp(last, 0, total - 1);
        const int first = std::max(0, last - height + 1);
        for (int i = first; i <= last; ++i) {
            const Line& line = lines_[filtered_[i]];
            const Color color = (line.color == Color::Default) ? theme.text : line.color;
            row_elements.push_back(text(line.text) | ftxui::color(color));
        }
    } else if (!filter_.empty()) {
        row_elements.push_back(text("(no lines match filter: " + filter_ + ")") | dim);
    } else {
        row_elements.push_back(text(""));
    }

    return vbox(std::move(row_elements)) | reflect(box_);
}

} // namespace TUIKIT
