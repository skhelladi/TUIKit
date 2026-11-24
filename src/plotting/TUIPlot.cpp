#include "tuikit/plotting/TUIPlot.h"
#include "tuikit/core/TUIStyle.h"
#include <ftxui/component/event.hpp>
#include <algorithm>
#include <climits>
#include <cmath>
#include <utility>

namespace TUIKIT {

namespace {

using LineStyle = TUIPlotLineStyle;
using Point = std::pair<int, int>;

// Named colors: rendered on every terminal (no truecolor required).
const ftxui::Color kPalette[] = {
    ftxui::Color::Yellow,     ftxui::Color::Cyan,      ftxui::Color::Green,      ftxui::Color::Magenta,
    ftxui::Color::Red,        ftxui::Color::Blue,      ftxui::Color::GrayLight,  ftxui::Color::GreenLight,
    ftxui::Color::RedLight,   ftxui::Color::MagentaLight, ftxui::Color::CyanLight, ftxui::Color::YellowLight};

// Keyboard events only reach focusable components and a plain Renderer is not one:
// this pass-through component makes the plot focusable.
class FocusableComponent : public ftxui::ComponentBase {
public:
    explicit FocusableComponent(ftxui::Component child) { Add(std::move(child)); }
    ftxui::Element OnRender() override { return ChildAt(0)->Render(); }
    bool Focusable() const override { return true; }
};

std::string glyph(TUIPlotMarker marker) {
    switch (marker) {
        case TUIPlotMarker::Circle: return "●";
        case TUIPlotMarker::Square: return "■";
        case TUIPlotMarker::Triangle: return "▲";
        case TUIPlotMarker::Diamond: return "◆";
        case TUIPlotMarker::Plus: return "+";
        case TUIPlotMarker::Cross: return "×";
        case TUIPlotMarker::None: break;
    }
    return "";
}

// Braille pixel coordinates: 2 x 4 pixels per cell.
void dot(ftxui::Canvas& canvas, int x, int y, const ftxui::Color& color) {
    if (x < 0 || y < 0 || x >= canvas.width() || y >= canvas.height()) return;  // pixel coordinates
    canvas.DrawPoint(x, y, true, color);
}

// Splits an UTF-8 string into one string per code point.
std::vector<std::string> glyphs(const std::string& text) {
    std::vector<std::string> out;
    for (std::size_t i = 0; i < text.size();) {
        const unsigned char lead = static_cast<unsigned char>(text[i]);
        const std::size_t length = lead < 0x80 ? 1 : (lead >> 5) == 0x6 ? 2 : (lead >> 4) == 0xE ? 3 : (lead >> 3) == 0x1E ? 4 : 1;
        out.push_back(text.substr(i, std::min(length, text.size() - i)));
        i += length;
    }
    return out;
}

int textWidth(const std::string& text) { return static_cast<int>(glyphs(text).size()); }  // in cells

// One code point per cell; cells outside the canvas are dropped.
void text(ftxui::Canvas& canvas, int column, int row, const std::string& value, const ftxui::Color& color) {
    const int columns = canvas.width() / 2;
    if (row < 0 || row >= canvas.height() / 4) return;
    const auto cells = glyphs(value);
    for (std::size_t i = 0; i < cells.size(); ++i) {
        const int x = column + static_cast<int>(i);
        if (x >= 0 && x < columns) canvas.DrawText(2 * x, 4 * row, cells[i], color);
    }
}

// Bresenham line with dash pattern and thickness 1..3.
void line(ftxui::Canvas& canvas, double x0, double y0, double x1, double y1, const ftxui::Color& color, LineStyle style, int width) {
    if (style == LineStyle::None) return;
    int x = static_cast<int>(std::lround(x0)), y = static_cast<int>(std::lround(y0));
    const int xe = static_cast<int>(std::lround(x1)), ye = static_cast<int>(std::lround(y1));
    const int dx = std::abs(xe - x), sx = x < xe ? 1 : -1;
    const int dy = -std::abs(ye - y), sy = y < ye ? 1 : -1;
    const int period = style == LineStyle::Solid ? 1 : style == LineStyle::Dashed ? 8 : 3;
    const int on = style == LineStyle::Dashed ? 5 : 1;
    int err = dx + dy;
    for (int n = 0;; ++n) {
        if (n % period < on) {
            dot(canvas, x, y, color);
            if (width >= 2) dot(canvas, x, y + 1, color);
            if (width >= 3) {
                dot(canvas, x + 1, y, color);
                dot(canvas, x + 1, y + 1, color);
            }
        }
        if (x == xe && y == ye) break;
        const int e2 = 2 * err;
        if (e2 >= dy) {
            err += dy;
            x += sx;
        }
        if (e2 <= dx) {
            err += dx;
            y += sy;
        }
    }
}

// Liang-Barsky clip of a segment to a rectangle; false when fully outside.
bool clip(double& x0, double& y0, double& x1, double& y1, double xmin, double xmax, double ymin, double ymax) {
    const double dx = x1 - x0, dy = y1 - y0;
    const double p[4] = {-dx, dx, -dy, dy};
    const double q[4] = {x0 - xmin, xmax - x0, y0 - ymin, ymax - y0};
    double t0 = 0.0, t1 = 1.0;
    for (int i = 0; i < 4; ++i) {
        if (p[i] == 0.0) {
            if (q[i] < 0.0) return false;
            continue;
        }
        const double r = q[i] / p[i];
        if (p[i] < 0.0) t0 = std::max(t0, r);
        else t1 = std::min(t1, r);
        if (t0 > t1) return false;
    }
    x0 += t0 * dx;
    y0 += t0 * dy;
    x1 -= (1.0 - t1) * dx;
    y1 -= (1.0 - t1) * dy;
    return true;
}

// Keeps the min and max y of every cell column so spikes survive downsampling.
std::vector<Point> decimate(const std::vector<Point>& points, int pixel_min, int columns) {
    std::vector<int> low(columns, INT_MAX), high(columns, INT_MIN);
    for (const auto& point : points) {
        const int column = std::clamp((point.first - pixel_min) / 2, 0, columns - 1);
        low[column] = std::min(low[column], point.second);
        high[column] = std::max(high[column], point.second);
    }
    std::vector<Point> out;
    for (int column = 0; column < columns; ++column) {
        if (low[column] == INT_MAX) continue;
        out.emplace_back(pixel_min + 2 * column + 1, low[column]);
        out.emplace_back(pixel_min + 2 * column + 1, high[column]);
    }
    return out;
}

std::size_t nearest(const std::vector<double>& values, double x) {
    const auto it = std::lower_bound(values.begin(), values.end(), x);
    if (it == values.end()) return values.size() - 1;
    if (it == values.begin()) return 0;
    return (x - *(it - 1) <= *it - x) ? static_cast<std::size_t>(it - values.begin()) - 1 : static_cast<std::size_t>(it - values.begin());
}

} // namespace

TUIPlot::TUIPlot(const std::string& title, TUIWidget* /*parent*/)
    : TUIWidget(ftxui::Renderer([this] { return render(); })), title_(title) {
    component_ = std::make_shared<FocusableComponent>(ftxui::CatchEvent(component_, [this](const ftxui::Event& event) { return handleEvent(event); }));
}

std::shared_ptr<TUIPlotSeries> TUIPlot::addSeries(const std::string& name) {
    auto series = std::make_shared<TUIPlotSeries>(name);
    series->setColor(kPalette[series_.size() % (sizeof(kPalette) / sizeof(kPalette[0]))]);
    series_.push_back(series);
    return series;
}

void TUIPlot::setWindow(double factor, double shift) {
    if (!zoomed_) {
        zoom_lo_ = current_lo_;
        zoom_hi_ = current_hi_;
    }
    const double width = (zoom_hi_ - zoom_lo_) * factor;
    const double center = (zoom_lo_ + zoom_hi_) / 2.0 + shift * (zoom_hi_ - zoom_lo_);
    zoom_lo_ = center - width / 2.0;
    zoom_hi_ = center + width / 2.0;
    zoomed_ = true;
}

bool TUIPlot::handleEvent(const ftxui::Event& event) {
    using ftxui::Event;
    if (crosshair_ && (event == Event::ArrowLeft || event == Event::ArrowRight)) {
        const double step = (current_hi_ - current_lo_) / 50.0;
        crosshair_x_ = std::clamp(crosshair_x_ + (event == Event::ArrowLeft ? -step : step), current_lo_, current_hi_);
        return true;
    }
    if (event == Event::Character('+') || event == Event::Character('=')) { setWindow(0.7, 0.0); return true; }
    if (event == Event::Character('-')) { setWindow(1.4, 0.0); return true; }
    if (event == Event::Character(',')) { setWindow(1.0, -0.1); return true; }
    if (event == Event::Character('.')) { setWindow(1.0, 0.1); return true; }
    if (event == Event::Character('z')) { zoomed_ = false; return true; }
    if (event == Event::Character('l')) { legend_visible_ = !legend_visible_; return true; }
    if (event == Event::Character('c')) {
        crosshair_ = !crosshair_;
        crosshair_x_ = (current_lo_ + current_hi_) / 2.0;
        return true;
    }
    if (event == Event::Character('g')) {
        x_axis_.setMajorGrid(!x_axis_.majorGrid());
        y_axis_.setMajorGrid(x_axis_.majorGrid());
        return true;
    }
    return false;
}

ftxui::Element TUIPlot::render() {
    const auto& theme = TUIStyle::instance().currentTheme();
    ftxui::Element plot = ftxui::canvas([this](ftxui::Canvas& canvas) { draw(canvas); }) | ftxui::flex;
    std::vector<ftxui::Element> column;
    if (!title_.empty()) {
        column.push_back(ftxui::text(title_) | ftxui::bold | ftxui::color(theme.primary));
        column.push_back(ftxui::separator());
    }
    if (legend_visible_ && !series_.empty()) {  // legend on its own line, above the plot area
        std::vector<ftxui::Element> items;
        for (const auto& series : series_) {
            const std::string sample = std::string(series->lineStyle() == LineStyle::Dashed ? "┄" : series->lineStyle() == LineStyle::Dotted ? "┈" : series->lineStyle() == LineStyle::None ? " " : "─")
                + glyph(series->marker()) + " " + series->name() + "   ";
            items.push_back(ftxui::text(sample) | ftxui::color(series->visible() ? series->color() : theme.border));
        }
        column.push_back(ftxui::hbox(items));
    }
    column.push_back(plot);
    return ftxui::vbox(column) | ftxui::flex
        | ftxui::size(ftxui::WIDTH, ftxui::GREATER_THAN, min_columns_)
        | ftxui::size(ftxui::HEIGHT, ftxui::GREATER_THAN, min_rows_);
}

void TUIPlot::draw(ftxui::Canvas& canvas) {
    using ftxui::Color;
    const auto& theme = TUIStyle::instance().currentTheme();
    const int width = canvas.width() / 2, height = canvas.height() / 4;  // FTXUI >= 7: canvas size in braille pixels (2 x 4 per cell)
    if (width < 12 || height < 6) return;

    // 1. Visible x range: zoom window, else the axis policy applied to the data extent.
    double data_min = 0.0, data_max = 1.0;
    bool has_data = false;
    for (const auto& series : series_) {
        if (!series->visible() || series->empty()) continue;
        double xmin, xmax, ymin, ymax;
        series->bounds(xmin, xmax, ymin, ymax);
        data_min = has_data ? std::min(data_min, xmin) : xmin;
        data_max = has_data ? std::max(data_max, xmax) : xmax;
        has_data = true;
    }
    x_axis_.resolve(data_min, data_max);
    current_lo_ = zoomed_ ? zoom_lo_ : x_axis_.min();
    current_hi_ = zoomed_ ? zoom_hi_ : x_axis_.max();
    if (!(current_hi_ > current_lo_)) {
        current_lo_ = data_min;
        current_hi_ = data_min + 1.0;
    }

    // 2. Snapshot of every visible series inside the window; y range from the snapshots.
    struct Snapshot {
        TUIPlotSeries* series;
        std::vector<double> x, y;
    };
    std::vector<Snapshot> snapshots;
    double y_min = 0.0, y_max = 1.0;
    bool has_points = false;
    for (const auto& series : series_) {
        if (!series->visible()) continue;
        Snapshot snapshot{series.get(), {}, {}};
        series->copy(current_lo_, current_hi_, snapshot.x, snapshot.y);
        for (double value : snapshot.y) {
            y_min = has_points ? std::min(y_min, value) : value;
            y_max = has_points ? std::max(y_max, value) : value;
            has_points = true;
        }
        snapshots.push_back(std::move(snapshot));
    }
    y_axis_.resolve(y_min, y_max);
    const double xlo = current_lo_, xhi = current_hi_, ylo = y_axis_.min(), yhi = y_axis_.max();

    // 3. Layout in cells; plot area in braille pixels.
    const std::string& x_label = x_axis_.label();
    const std::string& y_label = y_axis_.label();
    const int info_rows = crosshair_ ? 1 : 0;  // reserved line for the crosshair readout
    const int x_label_rows = x_label.empty() ? 0 : 1;
    // The y label is stacked vertically along the plot when every glyph fits on its own
    // line, otherwise it is written horizontally on a line above the plot (never truncated).
    const auto y_glyphs = glyphs(y_label);
    const bool y_vertical = !y_glyphs.empty() && static_cast<int>(y_glyphs.size()) <= height - 1 - x_label_rows - info_rows;
    const int top = (!y_glyphs.empty() && !y_vertical) ? 1 : 0;
    const int rows = std::max(1, height - 1 - x_label_rows - info_rows - top);  // plot lines, between top and the tick labels
    const int x_ticks = std::max(2, (width - 1) / 9), y_ticks = std::max(2, rows / 2);
    int label_width = 0;
    for (double tick : y_axis_.ticks(y_ticks)) label_width = std::max(label_width, textWidth(y_axis_.format(tick)));
    const int left = (y_vertical ? 1 : 0) + label_width + 1;
    const int px0 = 2 * left, px1 = 2 * (width - 1) - 1, py0 = 4 * top, py1 = 4 * (top + rows) - 1;
    const int plot_columns = width - 1 - left;
    if (px1 - px0 < 8 || py1 - py0 < 8) return;
    auto X = [&](double x) { return px0 + (x - xlo) / (xhi - xlo) * (px1 - px0); };
    auto Y = [&](double y) { return py1 - (y - ylo) / (yhi - ylo) * (py1 - py0); };

    // 4. Grid, frame, tick labels.
    const Color axis_color = theme.text;
    const Color major_color = major_color_.value_or(theme.border);
    const Color minor_color = minor_color_.value_or(theme.border);
    if (y_axis_.minorGrid()) for (double tick : y_axis_.minorTicks(y_ticks, 4.0 * (yhi - ylo) / (py1 - py0))) line(canvas, px0, Y(tick), px1, Y(tick), minor_color, LineStyle::Dotted, 1);
    if (x_axis_.minorGrid()) for (double tick : x_axis_.minorTicks(x_ticks, 4.0 * (xhi - xlo) / (px1 - px0))) line(canvas, X(tick), py0, X(tick), py1, minor_color, LineStyle::Dotted, 1);
    if (y_axis_.majorGrid()) for (double tick : y_axis_.ticks(y_ticks)) line(canvas, px0, Y(tick), px1, Y(tick), major_color, LineStyle::Solid, 1);
    if (x_axis_.majorGrid()) for (double tick : x_axis_.ticks(x_ticks)) line(canvas, X(tick), py0, X(tick), py1, major_color, LineStyle::Solid, 1);
    line(canvas, px0, py0, px1, py0, axis_color, LineStyle::Solid, 1);
    line(canvas, px0, py1, px1, py1, axis_color, LineStyle::Solid, 1);
    line(canvas, px0, py0, px0, py1, axis_color, LineStyle::Solid, 1);
    line(canvas, px1, py0, px1, py1, axis_color, LineStyle::Solid, 1);
    for (double tick : x_axis_.ticks(x_ticks)) {
        const std::string label = x_axis_.format(tick);
        text(canvas, static_cast<int>(std::lround(X(tick) / 2.0)) - textWidth(label) / 2, top + rows + info_rows, label, axis_color);
    }
    for (double tick : y_axis_.ticks(y_ticks)) {
        const std::string label = y_axis_.format(tick);
        text(canvas, left - 1 - textWidth(label), std::min(top + rows - 1, static_cast<int>(std::lround(Y(tick) / 4.0))), label, axis_color);
    }
    if (!x_label.empty()) text(canvas, (width - textWidth(x_label)) / 2, height - 1, x_label, axis_color);
    if (y_vertical) {
        const int start = top + (rows - static_cast<int>(y_glyphs.size())) / 2;
        for (std::size_t i = 0; i < y_glyphs.size(); ++i) text(canvas, 0, start + static_cast<int>(i), y_glyphs[i], axis_color);
    } else if (!y_glyphs.empty()) {
        text(canvas, left, 0, y_label, axis_color);
    }

    // 5. Series: decimate to the plot width, clip to the plot area, draw lines and markers.
    for (const auto& snapshot : snapshots) {
        const TUIPlotSeries* series = snapshot.series;
        std::vector<Point> points;
        points.reserve(snapshot.x.size());
        for (std::size_t i = 0; i < snapshot.x.size(); ++i)
            points.emplace_back(static_cast<int>(std::lround(X(snapshot.x[i]))), static_cast<int>(std::lround(Y(snapshot.y[i]))));
        const std::vector<Point> drawn = points.size() > static_cast<std::size_t>(plot_columns) ? decimate(points, px0, plot_columns) : points;
        for (std::size_t i = 1; i < drawn.size(); ++i) {
            double x0 = drawn[i - 1].first, y0 = drawn[i - 1].second, x1 = drawn[i].first, y1 = drawn[i].second;
            if (clip(x0, y0, x1, y1, px0, px1, py0, py1)) line(canvas, x0, y0, x1, y1, series->color(), series->lineStyle(), series->lineWidth());
        }
        const std::string marker = glyph(series->marker());
        if (!marker.empty()) {
            for (std::size_t i = 0; i < points.size(); i += static_cast<std::size_t>(series->markerStride())) {
                const auto& point = points[i];
                if (point.first < px0 || point.first > px1 || point.second < py0 || point.second > py1) continue;
                text(canvas, point.first / 2, point.second / 4, marker, series->color());
            }
        }
    }

    // 6. Crosshair: vertical line and the nearest value of every series.
    if (crosshair_) {
        crosshair_x_ = std::clamp(crosshair_x_, xlo, xhi);
        line(canvas, X(crosshair_x_), py0, X(crosshair_x_), py1, axis_color, LineStyle::Dotted, 1);
        std::string info = x_axis_.format(crosshair_x_);
        for (const auto& snapshot : snapshots) {
            if (snapshot.x.empty()) continue;
            const std::size_t i = nearest(snapshot.x, crosshair_x_);
            info += "  " + snapshot.series->name() + "=" + y_axis_.format(snapshot.y[i]);
        }
        text(canvas, left, top + rows, info, axis_color);
    }
}

} // namespace TUIKIT
