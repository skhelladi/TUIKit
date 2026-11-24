#ifndef TUIKIT_TUIPLOTSERIES_H
#define TUIKIT_TUIPLOTSERIES_H

#include <ftxui/screen/color.hpp>
#include <algorithm>
#include <cstddef>
#include <deque>
#include <mutex>
#include <string>
#include <vector>

namespace TUIKIT {

enum class TUIPlotLineStyle { None, Solid, Dashed, Dotted };
enum class TUIPlotMarker { None, Circle, Square, Triangle, Diamond, Plus, Cross };

// One curve: points sorted by x plus its style. Thread-safe: append() may be called
// from a worker thread while the renderer takes snapshots (copy()/bounds()).
class TUIPlotSeries {
public:
    explicit TUIPlotSeries(const std::string& name = "");

    void append(double x, double y);
    void appendY(double y);  // x = last x + dx (see setDx)
    void clear();

    TUIPlotSeries& setName(const std::string& name) { name_ = name; return *this; }
    TUIPlotSeries& setColor(ftxui::Color color) { color_ = color; return *this; }
    TUIPlotSeries& setLineStyle(TUIPlotLineStyle style) { line_style_ = style; return *this; }
    TUIPlotSeries& setLineWidth(int width) { line_width_ = std::clamp(width, 1, 3); return *this; }
    TUIPlotSeries& setMarker(TUIPlotMarker marker) { marker_ = marker; return *this; }
    TUIPlotSeries& setMarkerStride(int stride) { marker_stride_ = std::max(1, stride); return *this; }
    TUIPlotSeries& setVisible(bool visible) { visible_ = visible; return *this; }
    TUIPlotSeries& setCapacity(std::size_t max_points) { capacity_ = max_points; return *this; }  // 0 = unlimited
    TUIPlotSeries& setDx(double dx) { dx_ = dx; return *this; }

    const std::string& name() const { return name_; }
    ftxui::Color color() const { return color_; }
    TUIPlotLineStyle lineStyle() const { return line_style_; }
    int lineWidth() const { return line_width_; }
    TUIPlotMarker marker() const { return marker_; }
    int markerStride() const { return marker_stride_; }
    bool visible() const { return visible_; }

    bool empty() const;
    std::size_t size() const;
    // Extent of the stored points (unchanged if empty).
    void bounds(double& xmin, double& xmax, double& ymin, double& ymax) const;
    // Copies the points with x in [xmin, xmax] plus one neighbour on each side.
    std::size_t copy(double xmin, double xmax, std::vector<double>& xs, std::vector<double>& ys) const;

private:
    mutable std::mutex mutex_;
    std::deque<double> x_, y_;
    std::size_t capacity_ = 0;
    double dx_ = 1.0;
    std::string name_;
    ftxui::Color color_ = ftxui::Color::Default;
    TUIPlotLineStyle line_style_ = TUIPlotLineStyle::Solid;
    int line_width_ = 1;
    TUIPlotMarker marker_ = TUIPlotMarker::None;
    int marker_stride_ = 1;
    bool visible_ = true;
};

} // namespace TUIKIT

#endif // TUIKIT_TUIPLOTSERIES_H
