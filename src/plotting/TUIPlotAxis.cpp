#include "tuikit/plotting/TUIPlotAxis.h"
#include <algorithm>
#include <cmath>
#include <cstdio>

namespace TUIKIT {

namespace {

double niceStep(double raw) {
    if (!(raw > 0.0)) return 1.0;
    const double magnitude = std::pow(10.0, std::floor(std::log10(raw)));
    const double normalized = raw / magnitude;
    const double nice = normalized < 1.5 ? 1.0 : normalized < 3.0 ? 2.0 : normalized < 7.0 ? 5.0 : 10.0;
    return nice * magnitude;
}

} // namespace

TUIPlotAxis& TUIPlotAxis::setRange(double lo, double hi) {
    mode_ = TUIPlotRangeMode::Fixed;
    user_lo_ = lo;
    user_hi_ = hi;
    return *this;
}

TUIPlotAxis& TUIPlotAxis::setAutoRange() {
    mode_ = TUIPlotRangeMode::Auto;
    return *this;
}

TUIPlotAxis& TUIPlotAxis::setFollowTail(double window) {
    mode_ = TUIPlotRangeMode::FollowTail;
    window_ = window;
    return *this;
}

TUIPlotAxis& TUIPlotAxis::setAutoLimits(bool on, double lo, double hi) {
    has_limits_ = on;
    lim_lo_ = lo;
    lim_hi_ = hi;
    return *this;
}

void TUIPlotAxis::resolve(double data_min, double data_max) {
    switch (mode_) {
        case TUIPlotRangeMode::Fixed:
            lo_ = user_lo_;
            hi_ = user_hi_;
            break;
        case TUIPlotRangeMode::FollowTail:
            hi_ = data_max;
            lo_ = data_max - window_;
            break;
        case TUIPlotRangeMode::Auto: {
            double margin = (data_max - data_min) * 0.05;
            if (margin <= 0.0) margin = std::max(1.0, std::abs(data_min) * 0.05);
            lo_ = data_min - margin;
            hi_ = data_max + margin;
            break;
        }
    }
    if (mode_ != TUIPlotRangeMode::Fixed && has_limits_) {
        lo_ = std::max(lo_, lim_lo_);
        hi_ = std::min(hi_, lim_hi_);
    }
    if (!(hi_ > lo_)) {  // empty or degenerate range
        lo_ = 0.0;
        hi_ = 1.0;
    }
}

double TUIPlotAxis::stepFor(int max_count) const {
    if (step_ > 0.0) return step_;
    step_cache_ = niceStep((hi_ - lo_) / std::max(1, max_count - 1));
    return step_cache_;
}

std::vector<double> TUIPlotAxis::ticks(int max_count) const {
    const double step = stepFor(max_count);
    const long first = static_cast<long>(std::ceil(lo_ / step - 1e-9));
    std::vector<double> out;
    for (long k = first; k - first < 10000; ++k) {
        const double value = k * step;
        if (value > hi_ + step * 1e-9) break;
        out.push_back(value);
    }
    return out;
}

std::vector<double> TUIPlotAxis::minorTicks(int max_count, double min_spacing) const {
    std::vector<double> out;
    if (!minor_grid_ || ndiv_ < 2) return out;
    const double major = stepFor(max_count);
    // Use the largest allowed number of divisions whose spacing is not below min_spacing.
    for (int divisions : {ndiv_, 4, 2}) {
        if (divisions > ndiv_ || divisions < 2 || major / divisions < min_spacing) continue;
        const double step = major / divisions;
        const long first = static_cast<long>(std::ceil(lo_ / step - 1e-9));
        for (long k = first; k - first < 10000 * divisions; ++k) {
            const double value = k * step;
            if (value > hi_ + step * 1e-9) break;
            if (k % divisions != 0) out.push_back(value);
        }
        break;
    }
    return out;
}

std::string TUIPlotAxis::format(double value) const {
    char buffer[64];
    if (!fmt_.empty()) {
        std::snprintf(buffer, sizeof buffer, fmt_.c_str(), value);
        return buffer;
    }
    const double magnitude = std::max(std::abs(lo_), std::abs(hi_));
    if (magnitude != 0.0 && (magnitude >= 1e5 || magnitude < 1e-3)) {
        std::snprintf(buffer, sizeof buffer, "%.1e", value);
    } else {
        const int decimals = step_cache_ >= 1.0 ? 0 : std::clamp(static_cast<int>(std::ceil(-std::log10(step_cache_))), 1, 6);
        std::snprintf(buffer, sizeof buffer, "%.*f", decimals, value);
    }
    return buffer;
}

} // namespace TUIKIT
