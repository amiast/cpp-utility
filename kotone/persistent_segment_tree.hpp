#ifndef KOTONE_PERSISTENT_SEGMENT_TREE
#define KOTONE_PERSISTENT_SEGMENT_TREE 1

#include <vector>
#include <cassert>

namespace kotone {

// A fully-persistent lazy segment tree for a sparse collection of values over an interval.
template <
    typename S,
    S (*op)(S, S),
    S (*e)(),
    typename F,
    S (*mapping)(F, S),
    F (*composition)(F, F),
    F (*id)()
> struct persistent_segment_tree {
  private:
    struct node {
        S val = e();
        F lazy = id();
        int left = -1, right = -1;
    };

    const int64_t _low, _high;
    std::vector<node> _nodes;

    int _copy(int index) {
        _nodes.push_back(_nodes[index]);
        return _nodes.size() - 1;
    }

    int _apply(int index, F f, bool copying) {
        if (copying) index = _copy(index);
        _nodes[index].val = mapping(f, _nodes[index].val);
        _nodes[index].lazy = composition(f, _nodes[index].lazy);
        return index;
    }

    void _push(int index) {
        F app = _nodes[index].lazy;
        if (_nodes[index].left == -1) {
            _nodes[index].left = _nodes.size();
            _nodes.emplace_back(mapping(app, e()), app);
        } else {
            _nodes[index].left = _apply(_nodes[index].left, app, true);
        }
        if (_nodes[index].right == -1) {
            _nodes[index].right = _nodes.size();
            _nodes.emplace_back(mapping(app, e()), app);
        } else {
            _nodes[index].right = _apply(_nodes[index].right, app, true);
        }
        _nodes[index].lazy = id();
    }

    void _update(int index) {
        S val_l = _nodes[_nodes[index].left].val;
        S val_r = _nodes[_nodes[index].right].val;
        _nodes[index].val = op(val_l, val_r);
    }

    void _set(int index, int64_t l, int64_t r, int64_t pos, S val) {
        if (l + 1 == r) {
            _nodes[index].val = val;
            return;
        }
        _push(index);
        int64_t m = (l + r) / 2;
        if (pos < m) _set(_nodes[index].left, l, m, pos, val);
        else _set(_nodes[index].right, m, r, pos, val);
        _update(index);
    }

    S _prod(int index, int64_t l, int64_t r, int64_t ql, int64_t qr) {
        if (qr <= l || r <= ql) return e();
        if (ql <= l && r <= qr) return _nodes[index].val;
        _push(index);
        int64_t m = (l + r) / 2;
        S val_l = _prod(_nodes[index].left, l, m, ql, qr);
        S val_r = _prod(_nodes[index].right, m, r, ql, qr);
        return op(val_l, val_r);
    }

    void _apply(int index, int64_t l, int64_t r, int64_t ql, int64_t qr, F app) {
        if (qr <= l || r <= ql) return;
        if (ql <= l && r <= qr) {
            _apply(index, app, false);
            return;
        }
        _push(index);
        int64_t m = (l + r) / 2;
        _apply(_nodes[index].left, l, m, ql, qr, app);
        _apply(_nodes[index].right, m, r, ql, qr, app);
        _update(index);
    }

    int _copy_paste(int source, int dest, int64_t l, int64_t r, int64_t ql, int64_t qr) {
        if (qr <= l || r <= ql) return dest;
        if (ql <= l && r <= qr) return source;
        _push(source);
        _push(dest);
        int64_t m = (l + r) / 2;
        _nodes[dest].left = _copy_paste(_nodes[source].left, _nodes[dest].left, l, m, ql, qr);
        _nodes[dest].right = _copy_paste(_nodes[source].right, _nodes[dest].right, m, r, ql, qr);
        _update(dest);
        return dest;
    }

  public:
    persistent_segment_tree() : _low(0), _high(0), _nodes(1) {}

    // Constructs a segment tree for the interval `[low, high)` with initial root `0`.
    // Requires `low <= high`.
    persistent_segment_tree(int64_t low, int64_t high) : _low(low), _high(high), _nodes(1) {
        assert(low <= high);
    }

    // Constructs a segment tree for the interval `[0, length)` with initial root `0`.
    // Requires `length >= 0`.
    persistent_segment_tree(int64_t length) : persistent_segment_tree(0, length) {}

    // Copies and returns the index of the specified segment tree.
    // Requires `root` to be a valid index of a root.
    int copy(int root) {
        assert(0 <= root && root < int(_nodes.size()));
        return _copy(root);
    }

    // Copies the specified segment tree and updates the value at the specified position.
    // Returns the index of the root of the copied tree.
    // Requires `root` to be a valid index of a root.
    // Requires `pos` to be within the segment tree's interval.
    int set(int root, int64_t pos, S val) {
        assert(0 <= root && root < int(_nodes.size()));
        assert(_low <= pos && pos < _high);
        int result = _copy(root);
        _set(result, _low, _high, pos, val);
        return result;
    }

    // Returns the product of the entire interval of the specified segment tree.
    // Requires `root` to be a valid index of a root.
    S all_prod(int root) const {
        assert(0 <= root && root < int(_nodes.size()));
        return _nodes[root].val;
    }

    // Returns the product of the interval `[low, high)` of the specified segment tree.
    // Requires `low` and `high` to be within the segment tree's interval.
    // Requires `low <= high`.
    S prod(int root, int64_t low, int64_t high) {
        assert(0 <= root && root < int(_nodes.size()));
        assert(_low <= low && low <= high && high <= _high);
        return _prod(root, _low, _high, low, high);
    }

    // Copies the specified segment tree and transforms the interval under the specified application.
    // Returns the index of the root of the copied tree.
    // Requires `root` to be a valid index of a root.
    // Requires `low` and `high` to be within the segment tree's interval.
    // Requires `low <= high`.
    int apply(int root, int64_t low, int64_t high, F app) {
        assert(0 <= root && root < int(_nodes.size()));
        assert(_low <= low && low <= high && high <= _high);
        int result = _copy(root);
        _apply(result, _low, _high, low, high, app);
        return result;
    }

    // Copies the specified interval from the `source` tree and pastes it onto the `dest` tree.
    // Returns the index of the root of the resulting tree.
    // Requires `source` and `dest` to be valid indices of a root.
    // Requires `low` and `high` to be within the segment tree's interval.
    // Requires `low <= high`.
    int copy_paste(int source, int dest, int64_t low, int64_t high) {
        assert(0 <= source && source < int(_nodes.size()));
        assert(0 <= dest && dest < int(_nodes.size()));
        assert(_low <= low && low <= high && high <= _high);
        if (low == _low && high == _high) return _copy(source);
        return _copy_paste(source, _copy(dest), _low, _high, low, high);
    }
};

}  // namespace kotone

#endif  // KOTONE_PERSISTENT_SEGMENT_TREE
