#ifndef KOTONE_GOLDEN_SEARCH_HPP
#define KOTONE_GOLDEN_SEARCH_HPP 1

#include <utility>
#include <cassert>

namespace kotone {

// Performs golden-section search and returns a pair `{x, f(x)}` for which `f(x)` is maximum in the interval `[low, high]`.
// Performs the specified number of steps such that the error is at most `(high - low) * pow(phi, -num_steps)`.
// Requires `low <= high`.
// Requires `num_steps >= 0`.
// Requires `T f(double x)` to be a concave function on the interval `[low, high]`.
template <typename T> std::pair<double, T> golden_search(double low, double high, int num_steps, const auto &f) {
    assert(low <= high);
    assert(num_steps >= 0);
    constexpr double INV_PHI_SQ = 0.38196601125010515;
    double ml = low + (high - low) * INV_PHI_SQ;
    double mr = high - (high - low) * INV_PHI_SQ;
    T vl = f(ml), vr = vl;
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
    return {ml, vl};
}

// Performs golden-section search and returns a pair `{x, f(x)}` for which `f(x)` is maximum in the interval `[low, high]`.
// Requires `low <= high`.
// Requires `T f(int64_t x)` to be a concave function on the interval `[low, high]`.
template <typename T> std::pair<int64_t, T> golden_search_discrete(int64_t low, int64_t high, const auto &f) {
    assert(low <= high);
    int64_t a = 1, b = 2;
    while (b < high - low + 2) a += b, std::swap(a, b);
    int64_t l = low - 1, m = l + b - a, r = l + a;
    T vm = f(m), vr = vm;
    bool chosen_low = true;
    while (m < r) {
        b -= a;
        std::swap(a, b);
        if (chosen_low && r <= high) vr = f(r);
        else if (!chosen_low) vm = f(m);
        if (r <= high && vm < vr) {
            l = m;
            m = r;
            vm = vr;
            r = l + a;
            chosen_low = true;
        } else {
            r = m;
            vr = vm;
            m = l + b - a;
            chosen_low = false;
        }
    }
    return {m, vm};
}

}  // namespace kotone

#endif  // KOTONE_GOLDEN_SEARCH_HPP
