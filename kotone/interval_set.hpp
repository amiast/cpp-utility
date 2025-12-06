#ifndef KOTONE_INTERVAL_SET_HPP
#define KOTONE_INTERVAL_SET_HPP 1

#include <vector>
#include <set>
#include <cassert>
#include <kotone/internal_type_traits>

namespace kotone {

// A data structure that manages non-overlapping intervals.
template <number T> struct interval_set {
    using interval = std::pair<T, T>;

  private:
    std::set<interval> _set;
    T _length{};

  public:
    // Constructs an empty interval set.
    interval_set() noexcept {}

    // Returns the number of disconnected intervals in the set.
    int num_intervals() const noexcept {
        return _set.size();
    }

    // Returns the sum of lengths of intervals in the set.
    T length() const noexcept {
        return _length;
    }

    // Inserts the interval `[l, r)` into the set.
    // Requires `l <= r`.
    void insert(T l, T r) {
        assert(l <= r);
        if (l == r) return;
        auto iter = _set.lower_bound({l, l});
        if (iter != _set.begin()) {
            auto prev = std::prev(iter);
            auto [L, R] = *prev;
            if (R >= r) return;
            if (R >= l) {
                _length -= R - L;
                l = L;
                _set.erase(prev);
            }
        }
        while (iter != _set.end() && iter->first <= r) {
            auto [L, R] = *iter;
            _length -= R - L;
            if (R > r) r = R;
            iter = _set.erase(iter);
        }
        _length += r - l;
        _set.emplace(l, r);
    }

    // Removes intervals in the range `[l, r)` and returns a vector containing the removed intervals.
    // Requires `l <= r`.
    std::vector<interval> remove(T l, T r) {
        assert(l <= r);
        if (l == r) return {};
        std::vector<interval> result;
        auto iter = _set.lower_bound({l, l});
        if (iter != _set.begin()) {
            auto prev = std::prev(iter);
            auto [L, R] = *prev;
            if (R > r) {
                _length -= r - l;
                _set.emplace(L, l);
                _set.emplace(r, R);
                _set.erase(prev);
                return {{l, r}};
            }
            if (R > l) {
                _length -= R - l;
                _set.emplace(L, l);
                result.emplace_back(l, R);
                _set.erase(prev);
            }
        }
        while (iter != _set.end() && iter->first < r) {
            auto [L, R] = *iter;
            if (R > r) {
                _length -= r - L;
                _set.emplace(r, R);
                result.emplace_back(L, r);
            } else {
                _length -= R - L;
                result.push_back(*iter);
            }
            iter = _set.erase(iter);
        }
        return result;
    }

    // Returns a vector containing the intervals in the set.
    std::vector<interval> to_vector() const {
        return {_set.begin(), _set.end()};
    }
};

}  // namespace kotone

#endif  // KOTONE_INTERVAL_SET_HPP
