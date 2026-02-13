#ifndef KOTONE_SPARSE_TABLE_HPP
#define KOTONE_SPARSE_TABLE_HPP 1

#include <vector>
#include <bit>
#include <cassert>

namespace kotone {

// A static data structure for computing interval product of idempotent monoids.
template <typename S, S (*op)(S, S), S (*e)()> struct sparse_table {
  private:
    int _size = 0, _depth = 0;
    std::vector<std::vector<S>> _table;

    static int _log(unsigned n) { return std::bit_width(n) - 1; }

  public:
    sparse_table() {}

    // Constructs a sparse table for the given vector.
    sparse_table(const std::vector<S> &vec) {
        if (vec.empty()) return;
        _size = vec.size();
        _depth = std::bit_width(vec.size());
        _table.assign(_depth, std::vector<S>(_size, e()));
        _table[0] = vec;
        for (int k = 1; k < _depth; k++) {
            for (int i = 0; i + (1 << k) <= _size; i++) {
                _table[k][i] = op(_table[k - 1][i], _table[k - 1][i + (1 << (k - 1))]);
            }
        }
    }

    // Returns the product of the specified interval `[l, r)`.
    // If `l == r`, returns `e()`.
    // Requires `0 <= l <= r <= size`.
    S prod(int l, int r) const {
        assert(0 <= l && l <= r && r <= _size);
        if (l == r) return e();
        int len = r - l;
        int k = _log(len);
        return op(_table[k][l], _table[k][r - (1 << k)]);
    }
};

// A static data structure for computing interval product of monoids.
// For idempotent monoids, consider using `sparse_table`.
template <typename S, S (*op)(S, S), S (*e)()> struct disjoint_sparse_table {
  private:
    int _size = 0, _depth = 0;
    std::vector<std::vector<S>> _table;

    static int _log(unsigned n) { return std::bit_width(n) - 1; }

  public:
    disjoint_sparse_table() {}

    // Constructs a sparse table for the given vector.
    disjoint_sparse_table(const std::vector<S> &vec) {
        if (vec.empty()) return;
        _size = vec.size();
        _depth = std::bit_width(vec.size());
        _table.assign(_depth, std::vector<S>(_size, e()));
        _table[0] = vec;
        for (int k = 1; k < _depth; k++) {
            int block_size = 1 << k;
            for (int l = 0; l < _size; l += block_size * 2) {
                int m = std::min(l + block_size, _size);
                int r = std::min(l + block_size * 2, _size);
                if (l < m) {
                    _table[k][m - 1] = vec[m - 1];
                    for (int i = m - 2; i >= l; i--) {
                        _table[k][i] = op(vec[i], _table[k][i + 1]);
                    }
                }
                if (m < r) {
                    _table[k][m] = vec[m];
                    for (int i = m + 1; i < r; i++) {
                        _table[k][i] = op(_table[k][i - 1], vec[i]);
                    }
                }
            }
        }
    }

    // Returns the product of the specified interval `[l, r)`.
    // If `l == r`, returns `e()`.
    // Requires `0 <= l <= r <= size`.
    S prod(int l, int r) const {
        assert(0 <= l && l <= r && r <= _size);
        if (l == r) return e();
        if (l + 1 == r) return _table[0][l];
        r--;
        int k = _log(l ^ r);
        return op(_table[k][l], _table[k][r]);
    }
};

}  // namespace kotone

#endif  // KOTONE_SPARSE_TABLE_HPP
