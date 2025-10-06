#ifndef KOTONE_SEGTREE_BEATS_HPP
#define KOTONE_SEGTREE_BEATS_HPP 1

#include <vector>
#include <bit>
#include <limits>
#include <cassert>
#include <kotone/internal_type_traits>

namespace kotone {

// A specialized segment tree that supports range updates with min/max values over a numerical sequence.
// Reference: https://nyaannyaan.github.io/library/segment-tree/segment-tree-beats.hpp
template <kotone::number T> struct segtree_beats {
  private:
    static constexpr T _MIN = std::numeric_limits<T>::lowest();
    static constexpr T _MAX = std::numeric_limits<T>::max();

    struct node {
        T sum{}, lazy_add{};
        T least{}, least_second = _MAX;
        T greatest{}, greatest_second = _MIN;
        int least_count = 1, greatest_count = 1;
    };

    std::vector<node> _vec;
    int _num_elements = 0, _len = 0, _depth = 0;

    static T _sum(T a, T b) { return a + b; }
    static T _min(T a, T b) { return a < b ? a : b; }
    static T _max(T a, T b) { return a > b ? a : b; }

    void _update(int i) {
        node &p = _vec[i], &l = _vec[i * 2], &r = _vec[i * 2 + 1];
        p.sum = l.sum + r.sum;
        if (l.least == r.least) {
            p.least = l.least;
            p.least_count = l.least_count + r.least_count;
            p.least_second = _min(l.least_second, r.least_second);
        } else {
            bool b = l.least < r.least;
            p.least = b ? l.least : r.least;
            p.least_count = b ? l.least_count : r.least_count;
            p.least_second = _min(b ? l.least_second : l.least, b ? r.least : r.least_second);
        }
        if (l.greatest == r.greatest) {
            p.greatest = l.greatest;
            p.greatest_count = l.greatest_count + r.greatest_count;
            p.greatest_second = _max(l.greatest_second, r.greatest_second);
        } else {
            bool b = l.greatest > r.greatest;
            p.greatest = b ? l.greatest : r.greatest;
            p.greatest_count = b ? l.greatest_count : r.greatest_count;
            p.greatest_second = _max(b ? l.greatest_second : l.greatest, b ? r.greatest : r.greatest_second);
        }
    }

    void _push_add(int i, T val) {
        node &n = _vec[i];
        n.sum += val * static_cast<T>(1 << (_depth - std::bit_width(static_cast<unsigned>(i)) + 1));
        n.least += val;
        n.greatest += val;
        if (n.least_second != _MAX) n.least_second += val;
        if (n.greatest_second != _MIN) n.greatest_second += val;
        n.lazy_add += val;
    }

    void _push_min(int i, T val) {
        node &n = _vec[i];
        n.sum += (val - n.greatest) * n.greatest_count;
        if (n.least == n.greatest) n.least = val;
        if (n.least_second == n.greatest) n.least_second = val;
        n.greatest = val;
    }

    void _push_max(int i, T val) {
        node &n = _vec[i];
        n.sum += (val - n.least) * n.least_count;
        if (n.greatest == n.least) n.greatest = val;
        if (n.greatest_second == n.least) n.greatest_second = val;
        n.least = val;
    }

    void _push(int i) {
        node &n = _vec[i];
        if (n.lazy_add != T{}) {
            _push_add(i * 2, n.lazy_add);
            _push_add(i * 2 + 1, n.lazy_add);
            n.lazy_add = T{};
        }
        if (n.least > _vec[i * 2].least) _push_max(i * 2, n.least);
        if (n.greatest < _vec[i * 2].greatest) _push_min(i * 2, n.greatest);
        if (n.least > _vec[i * 2 + 1].least) _push_max(i * 2 + 1, n.least);
        if (n.greatest < _vec[i * 2 + 1].greatest) _push_min(i * 2 + 1, n.greatest);
    }

    void _chmin_subtree(int i, T val) {
        node &n = _vec[i];
        if (n.greatest <= val) return;
        if (n.greatest_second < val) {
            _push_min(i, val);
            return;
        }
        _push(i);
        _chmin_subtree(i * 2, val);
        _chmin_subtree(i * 2 + 1, val);
        _update(i);
    }

    void _chmax_subtree(int i, T val) {
        node &n = _vec[i];
        if (n.least >= val) return;
        if (n.least_second > val) {
            _push_max(i, val);
            return;
        }
        _push(i);
        _chmax_subtree(i * 2, val);
        _chmax_subtree(i * 2 + 1, val);
        _update(i);
    }

    template <void (segtree_beats::*update_method)(int, T)> void _apply(int l, int r, T val) {
        if (l >= r) return;
        l += _len, r += _len;
        for (int d = _depth; d > 0; d--) {
            if (l & ((1 << d) - 1)) _push(l >> d);
            if (r & ((1 << d) - 1)) _push((r - 1) >> d);
        }
        int nl = l, nr = r;
        while (nl < nr) {
            if (nl & 1) (this->*update_method)(nl++, val);
            if (nr & 1) (this->*update_method)(--nr, val);
            nl >>= 1, nr >>= 1;
        }
        for (int d = 1; d <= _depth; d++) {
            if (l & ((1 << d) - 1)) _update(l >> d);
            if (r & ((1 << d) - 1)) _update((r - 1) >> d);
        }
    }

    static void _op_sum(T &val, const node &n) { val += n.sum; }
    static void _op_min(T &val, const node &n) { val = _min(val, n.least); }
    static void _op_max(T &val, const node &n) { val = _max(val, n.greatest); }

    template <void (*op_node)(T&, const node&), T e, T (*op)(T, T)> T _fold(int l, int r) {
        if (l >= r) return e;
        l += _len, r += _len;
        for (int d = _depth; d > 0; d--) {
            if (l & ((1 << d) - 1)) _push(l >> d);
            if (r & ((1 << d) - 1)) _push((r - 1) >> d);
        }
        T lval = e, rval = e;
        while (l < r) {
            if (l & 1) op_node(lval, _vec[l++]);
            if (r & 1) op_node(rval, _vec[--r]);
            l >>= 1, r >>= 1;
        }
        return op(lval, rval);
    }

  public:
    segtree_beats() {}

    // Constructs a segment tree beats with the specified initial values.
    segtree_beats(const std::vector<T> &vec) {
        if (vec.empty()) return;
        assert(vec.size() <= 100000000U);
        _num_elements = static_cast<int>(vec.size());
        _depth = std::bit_width(vec.size() - 1);
        _len = 1 << _depth;
        _vec.resize(_len * 2);
        for (unsigned i{}; i < vec.size(); i++) {
            _vec[i + _len].sum = _vec[i + _len].least = _vec[i + _len].greatest = vec[i];
        }
        for (int i = _len - 1; i > 0; i--) _update(i);
    }

    // Constructs a segment tree beats for the specified number of elements.
    // Initializes all elements to `T{}`.
    segtree_beats(int length) : segtree_beats(std::vector<T>(length)) {}

    // Constructs a segment tree beats for the specified number of elements.
    // Initializes all elements to `init_val`.
    segtree_beats(int length, T init_val) : segtree_beats(std::vector(length, init_val)) {}

    // Increments all elements with index `i` in `[l, r)` by `val`.
    // Requires `l` and `r` to be in bounds.
    void add(int l, int r, T val) {
        assert(0 <= l && l <= r && r <= _num_elements);
        _apply<&segtree_beats::_push_add>(l, r, val);
    }

    // Modifies all elements with index `i` in `[l, r)` to be at most `val`.
    // Requires `l` and `r` to be in bounds.
    void chmin(int l, int r, T val) {
        assert(0 <= l && l <= r && r <= _num_elements);
        _apply<&segtree_beats::_chmin_subtree>(l, r, val);
    }

    // Modifies all elements with index `i` in `[l, r)` to be at least `val`.
    // Requires `l` and `r` to be in bounds.
    void chmax(int l, int r, T val) {
        assert(0 <= l && l <= r && r <= _num_elements);
        _apply<&segtree_beats::_chmax_subtree>(l, r, val);
    }

    // Modifies all elements with index `i` in `[l, r)` to be between `low` and `high`.
    // Requires `l` and `r` to be in bounds.
    // Requires `low <= high`.
    void clamp(int l, int r, T low, T high) {
        assert(low <= high);
        chmin(l, r, high);
        chmax(l, r, low);
    }

    // Replaces all elements with index `i` in `[l, r)` with `val`.
    // Requires `l` and `r` to be in bounds.
    void set(int l, int r, T val) {
        clamp(l, r, val, val);
    }

    // Returns the sum of all elements with index `i` in `[l, r)`.
    // If `[l, r)` is empty, returns `T{}`.
    // Requires `l` and `r` to be in bounds.
    T sum(int l, int r) {
        assert(0 <= l && l <= r && r <= _num_elements);
        return _fold<_op_sum, T{}, _sum>(l, r);
    }

    // Returns the minimum of all elements with index `i` in `[l, r)`.
    // If `[l, r)` is empty, returns `std::numeric_limits<T>::max`.
    // Requires `l` and `r` to be in bounds.
    T min(int l, int r) {
        assert(0 <= l && l <= r && r <= _num_elements);
        return _fold<_op_min, _MAX, _min>(l, r);
    }

    // Returns the maximum of all elements with index `i` in `[l, r)`.
    // If `[l, r)` is empty, returns `std::numeric_limits<T>::lowest`.
    // Requires `l` and `r` to be in bounds.
    T max(int l, int r) {
        assert(0 <= l && l <= r && r <= _num_elements);
        return _fold<_op_max, _MIN, _max>(l, r);
    }
};

}  // namespace kotone

#endif  // KOTONE_SEGTREE_BEATS_HPP
