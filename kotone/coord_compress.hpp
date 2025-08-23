#ifndef COORD_COMPRESS_HPP
#define COORD_COMPRESS_HPP 1

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <cassert>

namespace kotone {

// A hash map that maintains the coordinate compression of a set.
// Supports custom comparators and hashes.
template <
    typename T,
    typename _comp_pred = std::less<T>,
    typename _hash = std::hash<T>,
    typename _key_eq_pred = std::equal_to<T>
>
struct coord_compress_hashmap {
  private:
    std::vector<T> _vals;
    std::unordered_map<T, int, _hash, _key_eq_pred> _map;
    std::unordered_set<T, _hash, _key_eq_pred> _erase;
    _comp_pred _comp{};
    _key_eq_pred _eq{};
    bool _requires_build = false;

    void _build() {
        if (!_requires_build) return;
        _requires_build = false;

        if (_erase.size()) {
            std::vector<T> temp_vals;
            temp_vals.reserve(_vals.size());
            for (T &val : _vals) {
                if (!_erase.contains(val)) temp_vals.push_back(std::move(val));
            }
            _erase.clear();
            _vals = std::move(temp_vals);
        }

        std::sort(_vals.begin(), _vals.end(), _comp);
        _vals.erase(std::unique(_vals.begin(), _vals.end(), _eq), _vals.end());

        _map.clear();
        int len = _vals.size();
        for (int i = 0; i < len; i++) {
            _map.emplace(_vals[i], i);
        }
    }

  public:
    // Inserts the given value into the hash map.
    void insert(T val) {
        _erase.erase(val);
        _vals.push_back(std::move(val));
        _requires_build = true;
    }

    // Removes the given value from the hash map.
    void erase(T val) {
        _erase.insert(std::move(val));
        _requires_build = true;
    }

    // Returns the compressed index of the given value.
    // If the value is not a member of the hash map, returns `-1` instead.
    int operator[](T val) {
        if (_requires_build) _build();
        if (_map.contains(val)) return _map[val];
        return -1;
    }

    // Returns a copy of the value at the specified index in the sorted order.
    // Requires the index to be in bounds.
    T get_nth(int index) {
        if (_requires_build) _build();
        assert(index >= 0);
        assert(index < _vals.size());
        return _vals[index];
    }

    // Returns the number of distinct elements in the hash map.
    int size() {
        if (_requires_build) _build();
        return _map.size();
    }
};

}  // namespace kotone

#endif  // COORD_COMPRESS
