#ifndef KOTONE_INTERVAL_MAP_HPP
#define KOTONE_INTERVAL_MAP_HPP 1

#include <vector>
#include <map>
#include <cassert>
#include <kotone/internal_type_traits>

namespace kotone {

// A data structure that manages non-overlapping intervals and their associated values.
template <number T, typename S> struct interval_map {
    using interval = std::pair<T, T>;

  private:
    std::map<interval, S> _map;
    T _length{};

  public:
    // Constructs an empty interval map.
    interval_map() noexcept {}

    // Returns the sum of lengths of intervals currently holding an associated value.
    T length() const noexcept {
        return _length;
    }

    // Inserts the interval `[l, r)` and its associated value into the map.
    // Overwrites existing intervals within `[l, r)`.
    // Requires `l <= r`.
    void insert(T l, T r, S val) {
        assert(l <= r);
        if (l == r) return;
        auto iter = _map.lower_bound({l, l});
        if (iter != _map.begin()) {
            auto prev = std::prev(iter);
            auto [L, R] = prev->first;
            if (R > r) {
                _map[{l, r}] = val;
                _map[{L, l}] = _map[{r, R}] = prev->second;
                _map.erase(prev);
                return;
            }
            if (R > l) {
                _length -= R - l;
                _map[{L, l}] = prev->second;
                _map.erase(prev);
            }
        }
        while (iter != _map.end() && iter->first.first < r) {
            auto [L, R] = iter->first;
            if (R > r) {
                _length -= r - L;
                _map[{r, R}] = iter->second;
            } else {
                _length -= R - L;
            }
            iter = _map.erase(iter);
        }
        _length += r - l;
        _map[{l, r}] = val;
    }

    // Removes intervals in the range `[l, r)` and returns a vector containing the removed intervals and their associated values.
    // Requires `l <= r`.
    std::vector<std::pair<interval, S>> remove(T l, T r) {
        assert(l <= r);
        if (l == r) return {};
        std::vector<std::pair<interval, S>> result;
        auto iter = _map.lower_bound({l, l});
        if (iter != _map.begin()) {
            auto prev = std::prev(iter);
            auto [L, R] = prev->first;
            if (R > r) {
                _length -= r - l;
                S val = prev->second;
                _map[{L, l}] = _map[{r, R}] = val;
                _map.erase(prev);
                return {{{l, r}, val}};
            }
            if (R > l) {
                _length -= R - l;
                _map[{L, l}] = prev->second;
                result.emplace_back(interval{l, R}, prev->second);
                _map.erase(prev);
            }
        }
        while (iter != _map.end() && iter->first.first < r) {
            auto [L, R] = iter->first;
            if (R > r) {
                _length -= r - L;
                _map[{r, R}] = iter->second;
                result.emplace_back(interval{L, r}, iter->second);
            } else {
                _length -= R - L;
                result.emplace_back(interval{L, R}, iter->second);
            }
            iter = _map.erase(iter);
        }
        return result;
    }

    // Returns a vector containing the intervals and their associated values in the map.
    std::vector<std::pair<interval, S>> to_vector() const {
        return {_map.begin(), _map.end()};
    }
};

}  // namespace kotone

#endif  // KOTONE_INTERVAL_MAP_HPP
