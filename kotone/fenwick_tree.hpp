#ifndef KOTONE_FENWICK_TREE_HPP
#define KOTONE_FENWICK_TREE_HPP 1

#include <vector>
#include <cassert>
#include <kotone/unordered_map>

namespace kotone {

// A Fenwick tree for a sparse collection of values over an interval.
template <typename T> struct fenwick_tree {
  private:
    int64_t _len{};
    unordered_map<int64_t, T> _map;

    T _sum(int64_t pos) const {
        T acc{};
        for (; pos; pos -= pos & -pos) {
            auto iter = _map.find(pos);
            if (iter == _map.end()) acc += T{};
            else acc += iter->second;
        }
        return acc;
    }

  public:
    fenwick_tree() {}

    // Constructs a Fenwick tree for the specified length.
    // Requires `length >= 0`.
    fenwick_tree(int64_t length) : _len(length + 1) {
        assert(length >= 0);
    }

    // Increments the value at the specified position.
    // Requires `pos` to be a valid index.
    void add(int64_t pos, T val) {
        assert(0 <= pos && pos + 1 < _len);
        for (++pos; pos < _len; pos += pos & -pos) _map[pos] += val;
    }

    // Returns the sum of the interval `[low, high)`.
    // Requires `low` and `high` to be within the Fenwick tree's interval.
    // Requires `low <= high`.
    T sum(int64_t low, int64_t high) const {
        assert(0 <= low && low <= high && high < _len);
        return _sum(high) - _sum(low);
    }
};

// A two-dimensional Fenwick tree for matrices with a small height and a large width.
// Reference: https://nyaannyaan.github.io/library/data-structure-2d/dynamic-binary-indexed-tree-2d.hpp
template <typename T> struct fenwick_tree_2d {
  private:
    using bit = fenwick_tree<T>;

    int _height{};
    int64_t _width{};
    std::vector<bit> _bits;

  public:
    fenwick_tree_2d() {}

    // Constructs a two-dimensional Fenwick tree for a matrix of the specified dimension.
    // Requires `height >= 0`.
    // Requires `width >= 0`.
    fenwick_tree_2d(int height, int64_t width) : _height(height + 1), _width(width), _bits(_height, bit(_width)) {
        assert(height >= 0);
        assert(width >= 0);
    }

    // Increments the value at position `(i, j)` in the matrix.
    // Requires `0 <= i < height`.
    // Requires `0 <= j < width`.
    void add(int i, int64_t j, T val) {
        assert(0 <= i && i + 1 < _height);
        assert(0 <= j && j < _width);
        for (++i; i < _height; i += i & -i) _bits[i].add(j, val);
    }

    // Returns the sum of the submatrix in `[0, i) * [0, j)`.
    // Requires `0 <= i <= height`.
    // Requires `0 <= j <= width`.
    T sum(int i, int64_t j) const {
        assert(0 <= i && i < _height);
        assert(0 <= j && j <= _width);
        T acc{};
        for (; i; i -= i & -i) {
            acc += _bits[i].sum(0, j);
        }
        return acc;
    }

    // Returns the sum of the submatrix in `[li, ri) * [lj, rj)`.
    // Requires `0 <= li <= ri <= height`.
    // Requires `0 <= lj <= rj <= width`.
    T sum(int li, int64_t lj, int ri, int64_t rj) const {
        assert(0 <= li && li <= ri && ri < _height);
        assert(0 <= lj && lj <= rj && rj <= _width);
        T acc{};
        while (li != ri) {
            if (li < ri) {
                acc += _bits[ri].sum(lj, rj);
                ri -= ri & -ri;
            } else {
                acc -= _bits[li].sum(lj, rj);
                li -= li & -li;
            }
        }
        return acc;
    }
};

}  // namespace kotone

#endif  // KOTONE_FENWICK_TREE
