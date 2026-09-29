// #include <kotone/misc/cht_line_container.cpp>
// https://github.com/amiast/cpp-utility

/**
 * CHT for signed 64-bit integers
 */

#include <set>

struct line { // ax + b
    mutable int64_t a, b, p;
    bool operator<(const line& other) const { return a < other.a; }
    bool operator<(int64_t x) const { return p < x; }
};

// Reference: https://github.com/kth-competitive-programming/kactl/blob/main/content/data-structures/LineContainer.h
struct cht_max : std::multiset<line, std::less<>> {
  private:
    static const int64_t _INF = ~(1LL << 63);

    int64_t _div(int64_t p, int64_t q) {
        return p / q - ((p ^ q) < 0 && p % q);
    }

    bool _isect(iterator x, iterator y) {
        if (y == end()) return x->p = _INF, false;
        if (x->a == y->a) x->p = x->b > y->b ? _INF : -_INF;
        else x->p = _div(y->b - x->b, x->a - y->a);
        return x->p >= y->p;
    }

  public:
    void add_affine(int64_t a, int64_t b) {
        auto z = emplace(a, b, 0), y = z++, x = y;
        while (_isect(y, z)) z = erase(z);
        if (x != begin() && _isect(--x, y)) _isect(x, y = erase(y));
        while ((y = x) != begin() && (--x)->p >= y->p) _isect(x, erase(y));
    }

    std::pair<int64_t, int64_t> query_max(int64_t x) {
        // assert(!empty());
        auto [a, b, _] = *lower_bound(x);
        return {a, b};
    }
};
