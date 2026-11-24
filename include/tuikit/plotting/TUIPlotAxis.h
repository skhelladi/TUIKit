#ifndef TUIKIT_TUIPLOTAXIS_H
#define TUIKIT_TUIPLOTAXIS_H

#include <algorithm>
#include <string>
#include <vector>

namespace TUIKIT {

enum class TUIPlotRangeMode { Fixed, Auto, FollowTail };

// One axis (X or Y): range policy, ticks, labels, grid toggles. Pure C++, no FTXUI.
class TUIPlotAxis {
public:
    TUIPlotAxis& setRange(double lo, double hi);                 // fixed by the user
    TUIPlotAxis& setAutoRange();                                 // data extent +/- 5% (default)
    TUIPlotAxis& setFollowTail(double window);                   // sliding window of width `window` ending at the latest data
    TUIPlotAxis& setAutoLimits(bool on, double lo = 0.0, double hi = 0.0);  // clamp Auto/FollowTail ranges
    TUIPlotAxis& setLabel(const std::string& label) { label_ = label; return *this; }
    TUIPlotAxis& setTickFormat(const std::string& printf_format) { fmt_ = printf_format; return *this; }  // empty = automatic
    TUIPlotAxis& setTickStep(double step) { step_ = step; return *this; }  // 0 = automatic (1-2-5 steps)
    TUIPlotAxis& setMajorGrid(bool on) { major_grid_ = on; return *this; }
    TUIPlotAxis& setMinorGrid(bool on) { minor_grid_ = on; return *this; }
    TUIPlotAxis& setMinorDivisions(int divisions) { ndiv_ = std::max(2, divisions); return *this; }

    void resolve(double data_min, double data_max);  // compute the effective range
    double min() const { return lo_; }
    double max() const { return hi_; }
    const std::string& label() const { return label_; }
    bool majorGrid() const { return major_grid_; }
    bool minorGrid() const { return minor_grid_; }
    std::vector<double> ticks(int max_count) const;        // major ticks in [min, max]
    std::vector<double> minorTicks(int max_count, double min_spacing = 0.0) const;  // minor ticks, majors excluded
    std::string format(double value) const;

private:
    double stepFor(int max_count) const;

    TUIPlotRangeMode mode_ = TUIPlotRangeMode::Auto;
    double user_lo_ = 0.0, user_hi_ = 1.0, window_ = 10.0;
    bool has_limits_ = false;
    double lim_lo_ = 0.0, lim_hi_ = 0.0;
    std::string label_, fmt_;
    double step_ = 0.0;
    bool major_grid_ = true, minor_grid_ = false;
    int ndiv_ = 5;
    double lo_ = 0.0, hi_ = 1.0;
    mutable double step_cache_ = 0.1;
};

} // namespace TUIKIT

#endif // TUIKIT_TUIPLOTAXIS_H
