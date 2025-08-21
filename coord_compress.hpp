#ifndef COORD_COMPRESS_HPP
#define COORD_COMPRESS_HPP 1

#include <vector>
#include <unordered_map>
#include <algorithm>
#include <cassert>

namespace kotone {

// A struct for maintaining coordinate compression with a hash map.
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
    _comp_pred _comp;

  public:
    // Inserts a value to be compressed.
    void insert(const T &val) {
        _vals.emplace_back(val);
    }

    // Builds the hash map with compressed values in ascending order and returns the number of distinct values compressed.
    // Uses custom comparator specified in the template argument `_comp_pred` for sorting.
    int build() {
        if (_map.size()) _map.clear();
        std::sort(_vals.begin(), _vals.end(), _comp);
        _vals.erase(std::unique(_vals.begin(), _vals.end()), _vals.end());
        int len = _vals.size();
        for (int i = 0; i < len; i++) {
            _map[_vals[i]] = i;
        }
        return len;
    }

    // Returns the compressed ID of the given value.
    // Requires the value to be a member of the hash map.
    int operator[](const T &val) const {
        assert(_map.contains(val));
        return _map.at(val);
    }

    // Returns a copy of the value at the specified index in the sorted order.
    // Requires the index to be in bounds.
    // The behavior is undefined if the hash map is not built after new values are inserted.
    T get_nth(int index) const {
        assert(index >= 0);
        assert(index < _vals.size());
        return _vals[index];
    }
};

}  // namespace kotone

#endif  // COORD_COMPRESS
