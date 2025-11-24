#ifndef KOTONE_SEGMENT_TREE_HPP
#define KOTONE_SEGMENT_TREE_HPP 1

#include <vector>
#include <cassert>

namespace kotone {

// A segment tree for a sparse collection of values over an interval.
template <typename S, S (*op)(S, S), S (*e)()> struct segment_tree {
  private:
    struct node {
        S val = e();
        int left = -1, right = -1;
    };

    const int64_t _low, _high;
    std::vector<node> _nodes;

    void _set(int index, int64_t l, int64_t r, int64_t pos, S val) {
        if (l + 1 == r) {
            _nodes[index].val = val;
            return;
        }
        int64_t m = (l + r) / 2;
        if (pos < m) {
            if (_nodes[index].left == -1) {
                _nodes[index].left = _nodes.size();
                _nodes.emplace_back();
            }
            _set(_nodes[index].left, l, m, pos, val);
        } else {
            if (_nodes[index].right == -1) {
                _nodes[index].right = _nodes.size();
                _nodes.emplace_back();
            }
            _set(_nodes[index].right, m, r, pos, val);
        }
        S val_l = _nodes[index].left == -1 ? e() : _nodes[_nodes[index].left].val;
        S val_r = _nodes[index].right == -1 ? e() : _nodes[_nodes[index].right].val;
        _nodes[index].val = op(val_l, val_r);
    }

    S _prod(int index, int64_t l, int64_t r, int64_t ql, int64_t qr) const {
        if (index == -1 || qr <= l || r <= ql) return e();
        if (ql <= l && r <= qr) return _nodes[index].val;
        int64_t m = (l + r) / 2;
        S val_l = _prod(_nodes[index].left, l, m, ql, qr);
        S val_r = _prod(_nodes[index].right, m, r, ql, qr);
        return op(val_l, val_r);
    }

    template <typename G> int64_t _max_right(int index, int64_t l, int64_t r, int64_t ql, const G &g, S &acc) const {
        if (index == -1 || r <= ql) return r;
        if (ql <= l) {
            S new_acc = op(acc, _nodes[index].val);
            if (g(new_acc)) {
                acc = new_acc;
                return r;
            }
            if (l + 1 == r) return l;
        }
        int64_t m = (l + r) / 2;
        int64_t result = _max_right(_nodes[index].left, l, m, ql, g, acc);
        if (result < m) return result;
        return _max_right(_nodes[index].right, m, r, ql, g, acc);
    }

    template <typename G> int64_t _min_left(int index, int64_t l, int64_t r, int64_t qr, const G &g, S &acc) const {
        if (index == -1 || qr <= l) return l;
        if (r <= qr) {
            S new_acc = op(_nodes[index].val, acc);
            if (g(new_acc)) {
                acc = new_acc;
                return l;
            }
            if (l + 1 == r) return r;
        }
        int64_t m = (l + r) / 2;
        int64_t result = _min_left(_nodes[index].right, m, r, qr, g, acc);
        if (result > m) return result;
        return _min_left(_nodes[index].left, l, m, qr, g, acc);
    }

  public:
    segment_tree() : _low(0), _high(0), _nodes(1) {}

    // Constructs a segment tree for the interval `[low, high)`.
    // Requires `low <= high`.
    segment_tree(int64_t low, int64_t high) : _low(low), _high(high), _nodes(1) {
        assert(low <= high);
    }

    // Constructs a segment tree for the interval `[0, length)`.
    // Requires `length >= 0`.
    segment_tree(int64_t length) : segment_tree(0, length) {}

    // Updates the value at the specified position.
    // Requires `pos` to be within the segment tree's interval.
    void set(int64_t pos, S val) {
        assert(_low <= pos && pos < _high);
        _set(0, _low, _high, pos, val);
    }

    // Returns the product of the entire interval.
    S all_prod() const {
        return _nodes[0].val;
    }

    // Returns the product of the interval `[low, high)`.
    // Requires `low` and `high` to be within the segment tree's interval.
    // Requires `low <= high`.
    S prod(int64_t low, int64_t high) const {
        assert(_low <= low && low <= high && high <= _high);
        return _prod(0, _low, _high, low, high);
    }

    // Returns the maximum `high` in the segment tree's interval such that `g(prod(low, high)) == true`.
    // Requires `low` to be within the segment tree's interval.
    // Requires `bool g(S x)` to be a monotonic predicate.
    // Requires `g(e()) == true`.
    template <typename G> int64_t max_right(int64_t low, G g) const {
        assert(_low <= low && low <= _high);
        assert(g(e()));
        if (low == _high) return _high;
        S acc = e();
        return _max_right(0, _low, _high, low, g, acc);
    }

    // Returns the minimum `low` in the segment tree's interval such that `g(prod(low, high)) == true`.
    // Requires `high` to be within the segment tree's interval.
    // Requires `bool g(S x)` to be a monotonic predicate.
    // Requires `g(e()) == true`.
    template <typename G> int64_t min_left(int64_t high, G g) const {
        assert(_low <= high && high <= _high);
        assert(g(e()));
        if (high == _low) return _low;
        S acc = e();
        return _min_left(0, _low, _high, high, g, acc);
    }
};

}  // namespace kotone

#endif  // KOTONE_SEGMENT_TREE_HPP
