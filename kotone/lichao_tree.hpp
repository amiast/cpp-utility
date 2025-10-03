#ifndef KOTONE_LICHAO_TREE_HPP
#define KOTONE_LICHAO_TREE_HPP 1

#include <vector>
#include <unordered_map>
#include <utility>
#include <algorithm>
#include <bit>
#include <climits>
#include <cassert>

namespace kotone {

// Represents a line as an affine function of the form `y = ax + b`.
struct line {
  private:
    int64_t _a = 0, _b = 0;

  public:
    line() noexcept {}

    // Constructs a line of the form `y = ax + b`.
    // Requires `|a| <= 2e9 + 1000`.
    // Requires `|b| <= 4e18 + 1000`.
    line(int64_t a, int64_t b) : _a(a), _b(b) {
        assert(std::abs(a) <= 2'000'001'000LL);
        assert(std::abs(b) <= 4'000'000'000'000'001'000LL);
    }

    // Returns the slope of the line.
    int64_t a() const noexcept {
        return _a;
    }

    // Returns the `y`-intercept of the line.
    int64_t b() const noexcept {
        return _b;
    }

    // Returns the line as a pair `(a, b)`.
    std::pair<int64_t, int64_t> get() const noexcept {
        return {_a, _b};
    }

    // Returns the `y`-coordinate of the point on the line with the given `x`-coordinate.
    // Requires `|x| <= 2e9 + 1000`.
    int64_t eval(int64_t x) const {
        assert(std::abs(x) <= 2'000'001'000LL);
        return _a * x + _b;
    }

    // Constructs a `line` instance without checking against any constraints.
    // If the assigned values violate constraints, the behavior is undefined.
    static line raw(int64_t a, int64_t b) noexcept {
        line result;
        result._a = a;
        result._b = b;
        return result;
    }
};

// A data structure that maintains the minima of affine functions over an interval.
struct lichao_tree {
  private:
    int _len = 0;
    std::vector<int64_t> _xs;
    std::unordered_map<int64_t, int> _index_of;
    std::vector<line> _tree;

    void _insert(int node, int l, int r, line new_line) {
        int m = (l + r) / 2;
        line &curr = _tree[node];
        if (new_line.eval(_xs[m]) < curr.eval(_xs[m])) std::swap(new_line, curr);
        if (l + 1 == r) return;
        if (new_line.eval(_xs[l]) < curr.eval(_xs[l])) _insert(node * 2, l, m, new_line);
        if (new_line.eval(_xs[r - 1]) < curr.eval(_xs[r - 1])) _insert(node * 2 + 1, m, r, new_line);
    }

    void _insert_segment(int node, int l, int r, line new_line, int seg_l, int seg_r) {
        if (seg_r <= l || r <= seg_l) return;
        if (seg_l <= l && r <= seg_r) return _insert(node, l, r, new_line);
        int m = (l + r) / 2;
        _insert_segment(node * 2, l, m, new_line, seg_l, seg_r);
        _insert_segment(node * 2 + 1, m, r, new_line, seg_l, seg_r);
    }

    int64_t _find_min(int node, int l, int r, int index) const {
        if (l + 1 == r) return _tree[node].eval(_xs[index]);
        int m = (l + r) / 2;
        if (index < m) return std::min(_tree[node].eval(_xs[index]), _find_min(node * 2, l, m, index));
        else return std::min(_tree[node].eval(_xs[index]), _find_min(node * 2 + 1, m, r, index));
    }

  public:
    lichao_tree() noexcept {}

    // Constructs a Li Chao tree for the integer domain `[l, r)`.
    // Requires `r - l <= 1e8`.
    // Requires `|l|, |r| <= 2e9 + 1000`.
    lichao_tree(int64_t l, int64_t r) {
        assert(l < r);
        assert(r - l <= 100000000LL);
        assert(std::abs(l) <= 2'000'001'000LL);
        assert(std::abs(r) <= 2'000'001'000LL);
        if (l == r) return;
        _len = 1 << std::bit_width(uint64_t(r - l - 1));
        _xs.resize(_len, 2'000'001'000LL);
        for (int i = 0; i < r - l; i++) {
            _xs[i] = i + l;
            _index_of[i + l] = i;
        }
        _tree.assign(_len * 2, line::raw(0, LLONG_MAX));
    }

    // Constructs a Li Chao tree for the domain specified by `vec`.
    // Requires `|vec[i]| <= 2e9 + 1000` for all `i`.
    lichao_tree(std::vector<int64_t> vec) {
        if (vec.empty()) return;
        if (!std::is_sorted(vec.begin(), vec.end())) {
            std::sort(vec.begin(), vec.end());
        }
        vec.erase(std::unique(vec.begin(), vec.end()), vec.end());
        assert(vec.size() <= 100000000U);
        assert(-2'000'001'000LL <= vec.front() && vec.back() <= 2'000'001'000LL);
        _xs = std::move(vec);
        for (int i = 0; i < static_cast<int>(_xs.size()); i++) {
            _index_of[_xs[i]] = i;
        }
        _len = 1 << std::bit_width(_xs.size() - 1);
        _xs.resize(_len, 2'000'001'000LL);
        _tree.assign(_len * 2, line::raw(0, LLONG_MAX));
    }

    // Inserts the specified line into the tree.
    void insert(line line) {
        if (_len == 0) return;
        _insert(1, 0, _len, line);
    }

    // Inserts line y = ax + b in to the tree.
    // Requires `|a| <= 2e9 + 1000`.
    // Requires `|b| <= 4e18 + 1000`.
    void insert(int64_t a, int64_t b) {
        insert({a, b});
    }

    // Inserts the specified line segment over `[l, r)` into the tree.
    void insert_segment(line line, int64_t l, int64_t r) {
        int index_l = std::distance(_xs.begin(), std::lower_bound(_xs.begin(), _xs.end(), l));
        int index_r = std::distance(_xs.begin(), std::lower_bound(_xs.begin(), _xs.end(), r));
        if (index_l >= index_r) return;
        _insert_segment(1, 0, _len, line, index_l, index_r);
    }

    // Inserts line segment `y = ax + b` for `x` in `[l, r)` into the tree.
    // Requires `|a| <= 2e9 + 1000`.
    // Requires `|b| <= 4e18 + 1000`.
    void insert_segment(int64_t a, int64_t b, int64_t l, int64_t r) {
        insert_segment({a, b}, l, r);
    }

    // Returns the minimum y-coordinate at the specified x-coordinate among all inserted lines.
    // If no lines are inserted, returns `LLONG_MAX`.
    // Requires `x` to be in the domain.
    int64_t find_min(int64_t x) {
        assert(_index_of.contains(x));
        return _find_min(1, 0, _len, _index_of[x]);
    }
};

}  // namespace kotone

#endif  // KOTONE_LICHAO_TREE_HPP
