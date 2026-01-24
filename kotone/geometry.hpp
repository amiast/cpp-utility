#ifndef KOTONE_GEOMETRY_HPP
#define KOTONE_GEOMETRY_HPP 1

#include <vector>
#include <utility>
#include <tuple>
#include <algorithm>
#include <compare>
#include <cassert>
#include <kotone/internal_type_traits>

namespace kotone {

// Represents a point or a vector on a two-dimensional plane.
template <signed_number T> struct point {
  private:
    T _x{}, _y{};

  public:
    point() {}
    point(T x, T y) : _x(x), _y(y) {}

    // Returns the coordinates as a pair.
    std::pair<T, T> get() const { return {_x, _y}; }

    // Returns the x-coordinate of the point.
    T x() const { return _x; }

    // Returns the y-coordinate of the point.
    T y() const { return _y; }

    point operator+(const point &other) const {
        return point(_x + other._x, _y + other._y);
    }

    point operator-() const {
        return point(-_x, -_y);
    }

    point operator-(const point &other) const {
        return *this + -other;
    }

    point operator*(T scalar) const {
        return point(_x * scalar, _y * scalar);
    }

    point operator/(T scalar) const {
        assert(scalar != T{});
        return point(_x / scalar, _y / scalar);
    }

    auto operator<=>(const point&) const = default;

    // Returns the dot product of two vectors.
    T dot(const point &other) const {
        return _x * other._x + _y * other._y;
    }

    // Returns the cross product of two vectors.
    T cross(const point &other) const {
        return _x * other._y - _y * other._x;
    }

    // Returns the squared L2 norm of the vector.
    T norm_squared() const {
        return _x * _x + _y * _y;
    }

    // Compares the arguments of two points relative to the origin.
    // Uses `[0, pi * 2)` as the range of arguments.
    // If the argument of `this` is smaller than `other`, returns `-1`.
    // If the arguments are the same, returns `0`.
    // Otherwise, returns `1`.
    // Requires `this->x() != 0 || this->y() != 0` and `other.x() != 0 || other.y() != 0`.
    int compare_args(const point &other) const {
        assert(_x != T{} || _y != T{});
        assert(other._x != T{} || other._y != T{});
        bool p = _y > 0 || (_y == 0 && _x > 0);
        bool q = other._y > 0 || (other._y == 0 && other._x > 0);
        if (p != q) return p ? -1 : 1;
        T c = cross(other);
        if (c == T{}) return 0;
        return c > T{} ? -1 : 1;
    }

    // A less-than (`<`) comparator struct using arguments.
    struct arg_less {
        bool operator()(const point &p, const point &q) const {
            return p.compare_args(q) == -1;
        }
    };

    // An equal-to (`==`) comparator struct using arguments.
    struct arg_eq {
        bool operator()(const point &p, const point &q) const {
            return p.compare_args(q) == 0;
        }
    };

    // A greater-than (`>`) comparator struct using arguments.
    struct arg_greater {
        bool operator()(const point &p, const point &q) const {
            return p.compare_args(q) == 1;
        }
    };
};

// Returns a tuple `{a, b, c}` representing the equation of a line `ax + by + c = 0`
// that passes through the two specified points.
// The function may return any valid equation that meets one of the following conditions:
// - `a > 0`, or
// - `a == 0 && b > 0`.
//
// Requires `(px, py) != (qx, qy)`.
template <signed_number T> std::tuple<T, T, T> linear_equation(T px, T py, T qx, T qy) {
    T dx = px - qx;
    T dy = py - qy;
    if (dy < 0) dx = -dx, dy = -dy;
    if (dy == 0 && dx > 0) dx = -dx;
    T c = py * dx - px * dy;
    return {dy, -dx, c};
}

// Returns a tuple `{a, b, c}` representing the equation of a line `ax + by + c = 0`
// that passes through the two specified points.
// The function may return any valid equation that meets one of the following conditions:
// - `a > 0`, or
// - `a == 0 && b > 0`.
//
// Requires `p != q`.
template <signed_number T> std::tuple<T, T, T> linear_equation(const point<T> &p, const point<T> &q) {
    return linear_equation(p.x(), p.y(), q.x(), q.y());
}

// Returns the lower half of the convex hull of the set of points in `vec`.
// The order of points in the returned convex hull is counterclockwise.
// Requires `vec` to be sorted.
template <signed_number T>
std::vector<point<T>> convex_hull_lower(const std::vector<point<T>> &vec) {
    std::vector<point<T>> lower;
    for (typename std::vector<point<T>>::const_iterator iter = vec.begin(); iter != vec.end(); iter++) {
        if (iter != vec.begin()) {
            assert(*std::prev(iter) <= *iter);
            if (*std::prev(iter) == *iter) continue;
        }
        while (lower.size() >= 2u) {
            point<T> a = *std::prev(lower.end(), 2);
            point<T> b = lower.back();
            if ((b - a).cross(*iter - a) > T{}) break;
            lower.pop_back();
        }
        lower.push_back(*iter);
    }
    return lower;
}

// Returns the upper half of the convex hull of the set of points in `vec`.
// The order of points in the returned convex hull is counterclockwise.
// Requires `vec` to be sorted.
template <signed_number T>
std::vector<point<T>> convex_hull_upper(const std::vector<point<T>> &vec) {
    std::vector<point<T>> upper;
    for (typename std::vector<point<T>>::const_reverse_iterator iter = vec.rbegin(); iter != vec.rend(); iter++) {
        if (iter != vec.rbegin()) {
            assert(*std::prev(iter) >= *iter);
            if (*std::prev(iter) == *iter) continue;
        }
        while (upper.size() >= 2u) {
            point<T> a = *std::prev(upper.end(), 2);
            point<T> b = upper.back();
            if ((b - a).cross(*iter - a) > T{}) break;
            upper.pop_back();
        }
        upper.push_back(*iter);
    }
    return upper;
}

// Returns the convex hull of the set of points in `vec`.
// The order of points in the returned convex hull is counterclockwise.
// Requires `vec` to be sorted.
template <signed_number T>
std::vector<point<T>> convex_hull(const std::vector<point<T>> &vec) {
    std::vector<point<T>> lower = convex_hull_lower(vec);
    if (lower.size() <= 1u) return lower;
    std::vector<point<T>> upper = convex_hull_upper(vec);
    lower.pop_back();
    upper.pop_back();
    lower.insert(lower.end(), upper.begin(), upper.end());
    return lower;
}

}  // namespace kotone

namespace std {

template <kotone::signed_number T> struct hash<kotone::point<T>> {
    std::size_t operator()(const kotone::point<T> &p) const {
        uint64_t x = static_cast<uint64_t>(p.x());
        uint64_t y = static_cast<uint64_t>(p.y());
        return std::hash<uint64_t>()((x >> 32 ^ x) << 32 | (y >> 32 ^ y));
    }
};

}  // namespace std

#endif  // KOTONE_GEOMETRY_HPP
