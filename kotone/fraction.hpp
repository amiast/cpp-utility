#ifndef KOTONE_FRACTION_HPP
#define KOTONE_FRACTION_HPP 1

#include <vector>
#include <numeric>
#include <cassert>

namespace kotone {

// A positive fraction represented as a vertex in the Stern-Brocot tree.
// Uses run-length encoding for path. A nonzero integer `d` in the RLE represents:
// - traversal to the `d`-th right descendant if `d > 0`; or
// - traversal to the `-d`-th left descendant if `d < 0`.
//
// Reference: https://nyaannyaan.github.io/library/math/stern-brocot-tree.hpp
template <std::signed_integral T> struct fraction {
  private:
    std::vector<T> _rle;
    T _lp = 0, _lq = 1, _mp = 1, _mq = 1, _rp = 1, _rq = 0;
    T _depth = 0;

    void _descend_left(T d) {
        if (_rle.empty() || _rle.back() > 0) _rle.push_back(0);
        _rle.back() += d;
        _depth -= d;
        _rp -= _lp * d, _rq -= _lq * d;
        _mp = _lp + _rp, _mq = _lq + _rq;
    }

    void _descend_right(T d) {
        if (_rle.empty() || _rle.back() < 0) _rle.push_back(0);
        _rle.back() += d;
        _depth += d;
        _lp += _rp * d, _lq += _rq * d;
        _mp = _lp + _rp, _mq = _lq + _rq;
    }

  public:
    // Constructs fraction at the root of the Stern-Brocot tree.
    fraction() noexcept {}

    // Constructs fraction from the specified numerator and denominator.
    // Requires `num >= 1 && denom >= 1`.
    fraction(T num, T denom) {
        assert(num >= 1);
        assert(denom >= 1);
        T g = std::gcd(num, denom);
        num /= g, denom /= g;
        while (num && denom) {
            if (num > denom) {
                T d = num / denom;
                num -= denom * d;
                descend(d - (num ? 0 : 1));
            } else {
                T d = denom / num;
                denom -= num * d;
                descend((denom ? 0 : 1) - d);
            }
        }
    }

    // Constructs fraction from the specified numerator-denominator pair.
    // Requires `frac.first >= 1 && frac.second >= 1`.
    fraction(const std::pair<T, T> &frac) : fraction(frac.first, frac.second) {}

    // Constructs fraction from the specified run-length encoding.
    fraction(const std::vector<T> &rle) {
        for (T d : rle) descend(d);
    }

    friend bool operator==(const fraction &l, const fraction &r) noexcept {
        return l._mp == r._mp && l._mq == r._mq;
    }

    friend auto operator<=>(const fraction &l, const fraction &r) noexcept {
        return l._mp * r._mq <=> l._mq * r._mp;
    }

    // Returns the value of the fraction as a numerator-denominator pair.
    std::pair<T, T> val() const noexcept { return {_mp, _mq}; }

    // Returns the lower bound of the vertex as a numerator-denominator pair.
    std::pair<T, T> lower_bound() const noexcept { return {_lp, _lq}; }

    // Returns the upper bound of the vertex as a numerator-denominator pair.
    std::pair<T, T> upper_bound() const noexcept { return {_rp, _rq}; }

    // Returns the depth of the vertex relative to the root.
    T depth() const noexcept { return _depth; }

    // Returns the run-length encoding for the path from the root to the current vertex.
    std::vector<T> rle() const { return _rle; }

    // Resets fraction to the root of the Stern-Brocot tree.
    void clear() noexcept {
        _rle.clear();
        _lp = 0, _lq = 1, _mp = 1, _mq = 1, _rp = 1, _rq = 0;
        _depth = 0;
    }

    // Traverses descendants by the specified number of steps.
    // If `steps > 0`, traverses the right descendants.
    // Otherwise, traverses the left descendants.
    void descend(T steps) {
        if (steps == 0) return;
        if (steps > 0) _descend_right(steps);
        else _descend_left(steps);
    }

    // Traverses ancestors by the specified number of steps.
    // Stops at the root if `steps` is greater than the current depth.
    // Requires `steps >= 0`.
    void ascend(T steps) {
        assert(steps >= 0);
        if (steps >= _depth) {
            clear();
            return;
        }
        _depth -= steps;
        while (steps) {
            if (_rle.back() > 0) {
                T d = std::min(steps, _rle.back());
                steps -= d;
                _mp -= _rp * d, _mq -= _rq * d;
                _lp = _mp - _rp, _lq = _mq - _rq;
                _rle.back() -= d;
            } else {
                T d = std::max(-steps, _rle.back());
                steps += d;
                _mp += _lp * d, _mq += _lq * d;
                _rp = _mp - _lp, _rq = _mq - _lq;
                _rle.back() -= d;
            }
            if (!_rle.back()) _rle.pop_back();
        }
    }

    // Returns the lowest common ancestor with `other`.
    fraction lca(const fraction &other) const {
        fraction a;
        for (std::size_t i = 0; i < _rle.size() && i < other._rle.size(); i++) {
            if ((_rle[i] > 0) != (other._rle[i] > 0)) break;
            if (_rle[i] > 0) a.descend(std::min(_rle[i], other._rle[i]));
            else a.descend(std::max(_rle[i], other._rle[i]));
            if (_rle[i] != other._rle[i]) break;
        }
        return a;
    }
};

// Given monotone predicate `bool f(std::pair<T, T> frac)` and integer `bound`, returns a pair containing:
// - `first`: the greatest positive fraction `x = {num, denom}` for which `f(x) == true && num <= bound && denom <= bound`;
// - `second`: the least fraction `y = {num, denom}` for which `f(y) == false && num <= bound && denom <= bound`.
//
// If `x` does not exist, returns `{{0, 1}, y}`.
// If `y` does not exist, returns `{x, {1, 0}}`.
//
// Requires `f` to be monotone.
// Requires `bound > 0`.
template <std::signed_integral T> std::pair<std::pair<T, T>, std::pair<T, T>> binary_search_rational(auto &f, T bound) {
    fraction<T> acc;
    auto exceeds = [&](bool ret) {
        auto [p, q] = acc.val();
        return p > bound || q > bound || f(acc.val()) == ret;
    };
    bool to_right = exceeds(true);
    while (true) {
        if (to_right) {
            T d = 1;
            while (true) {
                acc.descend(d);
                if (exceeds(false)) {
                    acc.ascend(d);
                    d /= 2;
                    break;
                }
                d *= 2;
            }
            while (d) {
                acc.descend(d);
                if (exceeds(false)) acc.ascend(d);
                d /= 2;
            }
            acc.descend(1);
        } else {
            T d = 1;
            while (true) {
                acc.descend(-d);
                if (exceeds(true)) {
                    acc.ascend(d);
                    d /= 2;
                    break;
                }
                d *= 2;
            }
            while (d) {
                acc.descend(-d);
                if (exceeds(true)) acc.ascend(d);
                d /= 2;
            }
            acc.descend(-1);
        }
        auto [p, q] = acc.val();
        if (p > bound || q > bound) return {acc.lower_bound(), acc.upper_bound()};
        to_right = !to_right;
    }
}

}  // namespace kotone

#endif  // KOTONE_FRACTION_HPP
