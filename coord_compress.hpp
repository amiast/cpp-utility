#ifndef COORD_COMPRESS_HPP
#define COORD_COMPRESS_HPP 1

#include <vector>
#include <unordered_map>
#include <algorithm>

// A struct for maintaining coordinate compression
template <typename T>
struct coord_compress {
  private:
    std::vector<T> _vals;
    std::unordered_map<T, int> _map;

  public:
    // Insert a value to be compressed.
    void insert(const T &val) {
        _vals.emplace_back(val);
    }

    // Compresses values in ascending order and returns the number of distinct values compressed.
    int build() {
        if (_map.size()) _map.clear();
        std::sort(_vals.begin(), _vals.end());
        _vals.erase(std::unique(_vals.begin(), _vals.end()), _vals.end());
        int len = _vals.size();
        for (int i = 0; i < len; i++) {
            _map[_vals[i]] = i;
        }
        return len;
    }

    // Returns the compressed ID of the given value.
    // The behavior is undefined if the value is not yet compressed.
    int operator[](const T &val) const {
        return _map.at(val);
    }
};

#endif  // COORD_COMPRESS
