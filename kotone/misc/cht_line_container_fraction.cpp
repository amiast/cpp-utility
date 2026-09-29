// #include <kotone/misc/cht_line_container_fraction.cpp>
// https://github.com/amiast/cpp-utility

/**
 * CHT for signed 32-bit rational numbers
 */

#include <set>

struct fraction {
    int64_t num, denom;
    auto operator<=>(const fraction &other) {
        return num * other.denom <=> other.num * denom;
    }
    auto operator<=>(int64_t b) {
        return num <=> b * denom;
    }
};

struct line { // ax + b
    mutable int64_t a, b;
    mutable fraction p;
    bool operator<(const line& other) const { return a < other.a; }
    bool operator<(const fraction &x) const { return p < x; }
};

// Reference: https://github.com/kth-competitive-programming/kactl/blob/main/content/data-structures/LineContainer.h
struct cht_max : std::multiset<line, std::less<>> {
  private:
    static const int64_t _INF = ~(1 << 31);

    fraction _div(int64_t p, int64_t q) {
        if (q < 0) p = -p, q = -q;
        return {p, q};
    }

    bool _isect(iterator x, iterator y) {
        if (y == end()) return x->p = {_INF, 1}, false;
        if (x->a == y->a) x->p = {x->b > y->b ? _INF : -_INF, 1};
        else x->p = _div(y->b - x->b, x->a - y->a);
        return x->p >= y->p;
    }

  public:
    void add_affine(int64_t a, int64_t b) {
        auto z = emplace(a, b, fraction{0, 1}), y = z++, x = y;
        while (_isect(y, z)) z = erase(z);
        if (x != begin() && _isect(--x, y)) _isect(x, y = erase(y));
        while ((y = x) != begin() && (--x)->p >= y->p) _isect(x, erase(y));
    }

    std::pair<int64_t, int64_t> query_max(fraction x) {
        // assert(!empty());
        auto [a, b, _] = *lower_bound(x);
        return {a, b};
    }
};
