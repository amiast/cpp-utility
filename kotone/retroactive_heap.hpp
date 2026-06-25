#ifndef KOTONE_RETROACTIVE_HEAP_HPP
#define KOTONE_RETROACTIVE_HEAP_HPP 1

#include <vector>
#include <set>
#include <bit>
#include <cassert>

namespace kotone {

// A partially retroactive heap.
// Maintains minimum elements by default and supports custom comparators.
// Optionally, pass the following functions to record values added to and deleted from the final heap:
// - `void on_add(T val)`
// - `void on_del(T val)`
//
// Reference: Erik D. Demaine, John Iacono, and Stefan Langerman. Retroactive Data Structures.
template <
    typename T,
    typename comp_pred = std::less<T>,
    void (*on_add)(T) = nullptr,
    void (*on_del)(T) = nullptr
> struct retroactive_heap {
  private:
    enum op_type {PUSH, POP, NOOP};
    struct seg_node { int sum = 0, pfx_min = 0, sfx_max = 0, min_add = 0, max_del = -1; };

    std::vector<op_type> _types;
    std::vector<T> _vals;
    std::vector<seg_node> _seg;
    std::multiset<T, comp_pred> _heap;
    int _age = 0, _len = 0;

    bool _comp(int i, int j) const {
        comp_pred cmp;
        if (i == 0) return false;
        if (j == 0) return true;
        return cmp(_vals[i], _vals[j]);
    }

    void _update(int i) {
        seg_node &curr = _seg[i], &left = _seg[i * 2], &right = _seg[i * 2 + 1];
        curr.sum = left.sum + right.sum;
        curr.pfx_min = std::min(left.pfx_min, left.sum + right.pfx_min);
        curr.sfx_max = std::max(left.sfx_max + right.sum, right.sfx_max);
        curr.min_add = (_comp(left.min_add, right.min_add) ? left : right).min_add;
        curr.max_del = (left.max_del != -1 && (right.max_del == -1 || _comp(right.max_del, left.max_del)) ? left : right).max_del;
    }

    int _next_bridge(int time) const {
        int sum = 0;
        for (int l = _len, r = time + 1 + _len; l < r; l >>= 1, r >>= 1) {
            if (~r & 1) continue;
            r--;
            sum += _seg[r].sum;
        }
        if (sum == 0) return time;
        int l = time + 1 + _len;
        do {
            while (~l & 1) l >>= 1;
            if (sum + _seg[l].pfx_min == 0) {
                while (l < _len) {
                    l <<= 1;
                    if (sum + _seg[l].pfx_min > 0) {
                        sum += _seg[l].sum;
                        l++;
                    }
                }
                return l - _len;
            }
            sum += _seg[l].sum;
            l++;
        } while ((l & -l) != l);
        return _age - 1;
    }

    int _prev_bridge(int time) const {
        int sum = 0;
        for (int l = time + _len, r = _len * 2; l < r; l >>= 1, r >>= 1) {
            if (~l & 1) continue;
            sum += _seg[l].sum;
            l++;
        }
        if (sum == 0) return time;
        int r = time + _len;
        do {
            r--;
            while (r > 1 && r & 1) r >>= 1;
            if (_seg[r].sfx_max + sum == 0) {
                while (r < _len) {
                    r <<= 1;
                    r++;
                    if (_seg[r].sfx_max + sum < 0) {
                        sum += _seg[r].sum;
                        r--;;
                    }
                }
                return r - _len;
            }
            sum += _seg[r].sum;
        } while ((r & -r) != r);
        return 0;
    }

    int _get_min_add(int time) const {
        int index = 0, bridge = _next_bridge(time);
        for (int l = _len, r = bridge + 1 + _len; l < r; l >>= 1, r >>= 1) {
            if (~r & 1) continue;
            r--;
            if (_comp(_seg[r].min_add, index)) index = _seg[r].min_add;
        }
        return index;
    }

    int _get_max_del(int time) const {
        int index = -1, bridge = _prev_bridge(time);
        for (int l = bridge + _len, r = _len * 2; l < r; l >>= 1, r >>= 1) {
            if (~l & 1) continue;
            if (_seg[l].max_del != -1 && (index == -1 || _comp(index, _seg[l].max_del))) index = _seg[l].max_del;
            l++;
        }
        return index;
    }

    void _push(int time) {
        _types[time] = PUSH;
        seg_node &curr = _seg[time + _len];
        int i = _get_max_del(time);
        if (i == -1 || _comp(i, time)) {
            _heap.insert(_vals[time]);
            if constexpr (on_add) on_add(_vals[time]);
            curr.min_add = time;
            for (int j = (time + _len) >> 1; j; j >>= 1) _update(j);
            return;
        }
        curr.max_del = time;
        curr.sum = curr.sfx_max = 1;
        for (int j = (time + _len) >> 1; j; j >>= 1) _update(j);
        seg_node &node = _seg[i + _len];
        if (i == 0) {
            node.sum--;
            node.sfx_max--;
            if (node.sum == 0) node.max_del = -1;
        } else {
            _heap.insert(_vals[i]);
            if constexpr (on_add) on_add(_vals[i]);
            node.min_add = i;
            node.max_del = -1;
            node.sum = node.sfx_max = 0;
        }
        for (int j = (i + _len) >> 1; j; j >>= 1) _update(j);
    }

    void _unpush(int time) {
        _types[time] = NOOP;
        seg_node &curr = _seg[time + _len];
        if (curr.sum == 0) {
            _heap.erase(_heap.find(_vals[time]));
            if constexpr (on_del) on_del(_vals[time]);
            curr.min_add = 0;
            for (int j = (time + _len) >> 1; j; j >>= 1) _update(j);
            return;
        }
        int i = _get_min_add(time);
        curr.max_del = -1;
        curr.sum = curr.sfx_max = 0;
        for (int j = (time + _len) >> 1; j; j >>= 1) _update(j);
        seg_node &node = _seg[i + _len];
        if (i > 0) {
            _heap.erase(_heap.find(_vals[i]));
            if constexpr (on_del) on_del(_vals[i]);
        }
        node.min_add = 0;
        node.max_del = i;
        node.sum++;
        node.sfx_max++;
        for (int j = (i + _len) >> 1; j; j >>= 1) _update(j);
    }

    void _pop(int time) {
        _types[time] = POP;
        int i = _get_min_add(time);
        seg_node &curr = _seg[time + _len], &node = _seg[i + _len];
        curr.sum = curr.pfx_min = -1;
        for (int j = (time + _len) >> 1; j; j >>= 1) _update(j);
        if (i > 0) {
            _heap.erase(_heap.find(_vals[i]));
            if constexpr (on_del) on_del(_vals[i]);
        }
        node.min_add = 0;
        node.max_del = i;
        node.sum++;
        node.sfx_max++;
        for (int j = (i + _len) >> 1; j; j >>= 1) _update(j);
    }

    void _unpop(int time) {
        _types[time] = NOOP;
        int i = _get_max_del(time);
        seg_node &curr = _seg[time + _len], &node = _seg[i + _len];
        curr.sum = curr.pfx_min = 0;
        for (int j = (time + _len) >> 1; j; j >>= 1) _update(j);
        if (i == 0) {
            node.sum--;
            node.sfx_max--;
            if (node.sum == 0) node.max_del = -1;
        } else {
            _heap.insert(_vals[i]);
            if constexpr (on_add) on_add(_vals[i]);
            node.min_add = i;
            node.max_del = -1;
            node.sum = node.sfx_max = 0;
        }
        for (int j = (i + _len) >> 1; j; j >>= 1) _update(j);
    }

  public:
    retroactive_heap() {}

    // Initializes a retroactive heap with no-operations for the specified age (number of operations).
    // Requires `age >= 0`.
    retroactive_heap(int age) : _age(age + 1) {
        assert(age >= 0);
        _types.assign(_age, NOOP);
        _vals.resize(_age);
        _len = 1 << std::bit_width(_age - 1u);
        _seg.resize(_len * 2);
    }

    // Returns the total number of operations.
    int age() const noexcept { return _age - 1; }

    // Sets the operation at the specified time to push.
    // Requires `0 <= time < age()`.
    void set_push(int time, T val) {
        assert(0 <= time && time < age());
        time++;
        if (_types[time] == PUSH) _unpush(time);
        else if (_types[time] == POP) _unpop(time);
        _vals[time] = val;
        _push(time);
    }

    // Sets the operation at the specified time to pop.
    // Requires `0 <= time < age()`.
    void set_pop(int time) {
        assert(0 <= time && time < age());
        time++;
        if (_types[time] == PUSH) _unpush(time);
        else if (_types[time] == POP) _unpop(time);
        _pop(time);
    }

    // Resets the operation at the specified time to no-operation.
    // Requires `0 <= time < age()`.
    void reset(int time) {
        assert(0 <= time && time < age());
        time++;
        if (_types[time] == PUSH) _unpush(time);
        else if (_types[time] == POP) _unpop(time);
    }

    // Returns whether the heap is empty.
    bool empty() const noexcept {
        return _heap.empty();
    }

    // Returns the size of the heap.
    int size() const noexcept {
        return _heap.size();
    }

    // Returns a copy of the top element of the heap.
    // Requires the heap to be non-empty.
    T top() const {
        assert(!empty());
        return *_heap.begin();
    }
};

}  // namespace kotone

#endif  // KOTONE_RETROACTIVE_HEAP_HPP
