#include "tuikit/plotting/TUIPlotSeries.h"
#include <algorithm>

namespace TUIKIT {

TUIPlotSeries::TUIPlotSeries(const std::string& name) : name_(name) {}

void TUIPlotSeries::append(double x, double y) {
    std::lock_guard lock(mutex_);
    if (!x_.empty() && x < x_.back()) {
        const auto it = std::upper_bound(x_.begin(), x_.end(), x);
        const auto index = it - x_.begin();
        x_.insert(it, x);
        y_.insert(y_.begin() + index, y);
    } else {
        x_.push_back(x);
        y_.push_back(y);
    }
    if (capacity_ > 0) {
        while (x_.size() > capacity_) {
            x_.pop_front();
            y_.pop_front();
        }
    }
}

void TUIPlotSeries::appendY(double y) {
    double x;
    {
        std::lock_guard lock(mutex_);
        x = x_.empty() ? 0.0 : x_.back() + dx_;
    }
    append(x, y);
}

void TUIPlotSeries::clear() {
    std::lock_guard lock(mutex_);
    x_.clear();
    y_.clear();
}

bool TUIPlotSeries::empty() const {
    std::lock_guard lock(mutex_);
    return x_.empty();
}

std::size_t TUIPlotSeries::size() const {
    std::lock_guard lock(mutex_);
    return x_.size();
}

void TUIPlotSeries::bounds(double& xmin, double& xmax, double& ymin, double& ymax) const {
    std::lock_guard lock(mutex_);
    if (x_.empty()) return;
    xmin = x_.front();
    xmax = x_.back();
    ymin = *std::min_element(y_.begin(), y_.end());
    ymax = *std::max_element(y_.begin(), y_.end());
}

std::size_t TUIPlotSeries::copy(double xmin, double xmax, std::vector<double>& xs, std::vector<double>& ys) const {
    std::lock_guard lock(mutex_);
    xs.clear();
    ys.clear();
    auto first = std::lower_bound(x_.begin(), x_.end(), xmin);
    auto last = std::upper_bound(x_.begin(), x_.end(), xmax);
    if (first >= last) return 0;
    if (first != x_.begin()) --first;
    if (last != x_.end()) ++last;
    const auto y_first = y_.begin() + (first - x_.begin());
    xs.assign(first, last);
    ys.assign(y_first, y_first + xs.size());
    return xs.size();
}

} // namespace TUIKIT
