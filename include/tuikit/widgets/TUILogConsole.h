#ifndef TUIKIT_TUILOGCONSOLE_H
#define TUIKIT_TUILOGCONSOLE_H

#include "tuikit/core/TUIWidget.h"
#include <cstddef>
#include <deque>
#include <mutex>
#include <string>
#include <vector>
#include <ftxui/screen/color.hpp>

namespace TUIKIT {

/// Scrolling log console backed by a bounded ring buffer (oldest lines are
/// dropped when the capacity is reached). Only the visible window is rendered,
/// so it stays fast with tens of thousands of lines.
///
/// Features:
/// - follow-tail mode: sticks to the bottom, disengages as soon as the user
///   scrolls up and re-engages when the bottom is reached again;
/// - per-line color support;
/// - optional case-insensitive substring filter;
/// - mouse wheel + keyboard scrolling (arrows, PageUp/PageDown, Home/End).
///
/// append()/appendLines()/clear() are thread-safe, so worker threads (solver
/// engines) can stream output while the UI thread renders.
class TUILogConsole : public TUIWidget {
public:
    struct Line {
        std::string text;
        ftxui::Color color = ftxui::Color::Default; // Default = theme text color
    };

    explicit TUILogConsole(std::size_t capacity = 10000, TUIWidget* parent = nullptr);
    ~TUILogConsole() override = default;

    void append(const std::string& line);
    void append(const std::string& line, ftxui::Color color);
    void appendLines(const std::vector<std::string>& lines);
    void appendLines(const std::vector<Line>& lines);

    void clear();

    /// Only lines containing the given substring (case-insensitive) are shown.
    void setFilter(const std::string& filter);
    std::string filter() const;

    /// Follow-tail mode (autoscroll to the bottom on new lines).
    void setFollow(bool follow);
    bool following() const;

    void setCapacity(std::size_t capacity);
    std::size_t capacity() const;
    std::size_t lineCount() const;

private:
    void appendLine(Line line);
    void rebuildFiltered() const;
    void scrollBy(int delta); // positive = towards older lines
    bool handleEvent(ftxui::Event event);
    ftxui::Element render();

    std::deque<Line> lines_;
    std::size_t capacity_ = 10000;

    // Filtered view: indices into lines_. Cached and rebuilt lazily.
    mutable std::vector<std::size_t> filtered_;
    mutable bool filtered_dirty_ = true;
    std::string filter_;

    bool follow_ = true;
    int scroll_offset_ = 0; // visible lines above the bottom; 0 == tail
    ftxui::Box box_;
    int visible_height_ = 24; // learned from the previous rendered frame

    mutable std::mutex mutex_;
};

} // namespace TUIKIT

#endif // TUIKIT_TUILOGCONSOLE_H
