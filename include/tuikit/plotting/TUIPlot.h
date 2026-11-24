#ifndef TUIKIT_TUIPLOT_H
#define TUIKIT_TUIPLOT_H

#include "tuikit/core/TUIWidget.h"
#include "tuikit/plotting/TUIPlotAxis.h"
#include "tuikit/plotting/TUIPlotSeries.h"
#include <ftxui/dom/canvas.hpp>
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace TUIKIT {

// Terminal plot: several series on one graph, fixed/auto/follow-tail ranges, major/minor
// grid, legend, crosshair, zoom and pan. Push data with TUIPlotSeries::append(), then call
// TUIApp::PostEvent(ftxui::Event::Custom) to request a repaint.
class TUIPlot : public TUIWidget {
public:
    static constexpr const char* kHelp =
        "+/-: zoom  ,/.: pan  z: reset  c: crosshair (left/right)  l: legend  g: grid";

    explicit TUIPlot(const std::string& title = "", TUIWidget* parent = nullptr);
    ~TUIPlot() override = default;

    std::shared_ptr<TUIPlotSeries> addSeries(const std::string& name);
    TUIPlotAxis& xAxis() { return x_axis_; }
    TUIPlotAxis& yAxis() { return y_axis_; }

    void setTitle(const std::string& title) { title_ = title; }
    void setLegendVisible(bool visible) { legend_visible_ = visible; }
    void setMinimumSize(int columns, int rows) { min_columns_ = columns; min_rows_ = rows; }
    // Empty optionals = theme border color (minor grid is drawn dotted).
    void setGridColors(std::optional<ftxui::Color> major, std::optional<ftxui::Color> minor) { major_color_ = major; minor_color_ = minor; }

private:
    ftxui::Element render();
    void draw(ftxui::Canvas& canvas);
    bool handleEvent(const ftxui::Event& event);
    void setWindow(double factor, double shift);

    std::string title_;
    std::vector<std::shared_ptr<TUIPlotSeries>> series_;
    TUIPlotAxis x_axis_, y_axis_;
    bool legend_visible_ = true, crosshair_ = false, zoomed_ = false;
    int min_columns_ = 40, min_rows_ = 16;  // minimum size of the whole widget
    double crosshair_x_ = 0.0, current_lo_ = 0.0, current_hi_ = 1.0, zoom_lo_ = 0.0, zoom_hi_ = 1.0;
    std::optional<ftxui::Color> major_color_, minor_color_;
};

} // namespace TUIKIT

#endif // TUIKIT_TUIPLOT_H
