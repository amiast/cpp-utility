#ifndef KOTONE_GOLDEN_SEARCH_HPP
#define KOTONE_GOLDEN_SEARCH_HPP 1

#include <cassert>

namespace kotone {

// Performs golden-section search and returns the value `x` that maximizes `f(x)` in the interval `[low, high]`.
// Performs the specified number of steps such that the error is at most `phi ** num_steps / 2`.
// Requires `low <= high`.
// Requires `num_steps >= 0`.
// Requires `f(double x)` to be a concave function on the interval `[low, high]`.
double golden_search(double low, double high, int num_steps, const auto &f) {
    assert(low <= high);
    assert(num_steps >= 0);
    constexpr double INV_PHI_SQ = 0.38196601125010515;
    double ml = low + (high - low) * INV_PHI_SQ;
    double mr = high - (high - low) * INV_PHI_SQ;
    double vl = f(ml), vr = 0;
    bool chosen_low = true;
    while (num_steps--) {
        if (chosen_low) vr = f(mr);
        else vl = f(ml);
        if (vl < vr) {
            low = ml;
            ml = mr;
            vl = vr;
            mr = high - (high - low) * INV_PHI_SQ;
            chosen_low = true;
        } else {
            high = mr;
            mr = ml;
            vr = vl;
            ml = low + (high - low) * INV_PHI_SQ;
            chosen_low = false;
        }
    }
    return (low + high) * .5;
}

}  // namespace kotone

#endif  // KOTONE_GOLDEN_SEARCH_HPP
