#ifndef KOTONE_FORMAL_POWER_SERIES_HPP
#define KOTONE_FORMAL_POWER_SERIES_HPP 1

#include <vector>
#include <algorithm>
#include <cassert>
#include <atcoder/convolution>
#include <kotone/internal_type_traits>

namespace kotone {

// A formal power series (`fps`) inherited from `vector<mint>` that supports various operations.
template <compatible_modint mint, bool using_ntt = true> struct formal_power_series : std::vector<mint> {
    using fps = formal_power_series;
    using std::vector<mint>::vector;
    using std::vector<mint>::operator=;

    // Adds `other` to `*this`.
    fps& operator+=(const fps &other) {
        if (this->size() < other.size()) this->resize(other.size());
        for (std::size_t i = 0; i < other.size(); i++) (*this)[i] += other[i];
        return *this;
    }

    // Subtracts `other` from `*this`.
    fps& operator-=(const fps &other) {
        if (this->size() < other.size()) this->resize(other.size());
        for (std::size_t i = 0; i < other.size(); i++) (*this)[i] -= other[i];
        return *this;
    }

    // Multiplies `*this` by `other`.
    fps& operator*=(const fps &other) {
        if (this->empty() || other.empty()) this->clear();
        else if constexpr (using_ntt) *this = atcoder::convolution(*this, other);
        else {
            fps result(this->size() + other.size() - 1);
            for (std::size_t i = 0; i < this->size(); i++) {
                for (std::size_t j = 0; j < other.size(); j++) {
                    result[i + j] += (*this)[i] * other[j];
                }
            }
            *this = result;
        }
        return *this;
    }

    // Multiplies each coefficient by `m`.
    fps& operator*=(const mint &m) { for (mint &c : *this) c *= m; return *this; }
    // Divides each coefficient by `m`.
    // Requires `m != 0`.
    fps& operator/=(const mint &m) { assert(m != 0); for (mint &c : *this) c /= m; return *this; }

    // Returns a copy of `*this`.
    fps operator+() const { return *this; }
    // Returns a copy of negated `*this`.
    fps operator-() const { return fps{} - *this; }

    // Returns `*this` evaluated at `m`.
    mint operator()(const mint &m) const {
        mint result = 0, acc = 1;
        for (std::size_t i = 0; i < this->size(); i++) {
            result += (*this)[i] * acc;
            acc *= m;
        }
        return result;
    }

    // Strips trailing `0`'s.
    void strip() const { while (this->size() && this->back() == 0) this->pop_back(); }

    // Returns the sum of `a` and `b`.
    friend fps operator+(const fps &a, const fps &b) { fps c = a; return c += b; }
    // Returns the difference of `a` and `b`.
    friend fps operator-(const fps &a, const fps &b) { fps c = a; return c -= b; }
    // Returns the product of `a` and `b`.
    friend fps operator*(const fps &a, const fps &b) { fps c = a; return c *= b; }
    // Returns `a` with its coefficients multiplied by `b`.
    friend fps operator*(const fps &a, const mint &b) { fps c = a; return c *= b; }
    // Returns `b` with its coefficients multiplied by `a`.
    friend fps operator*(const mint &a, const fps &b) { return b * a; }
    // Returns `a` with its coefficients divided by `b`.
    // Requires `b != 0`.
    friend fps operator/(const fps &a, const mint &b) { assert(b != 0); fps c = a; return c /= b; }

    // Returns the inverse of `f` up to the first `n` coefficients.
    // Requires `!f.empty()`.
    // Requires `f[0] != 0`.
    // Requires `n >= 0`.
    static fps inverse(const fps &f, int n) {
        assert(!f.empty());
        assert(f[0] != 0);
        assert(n >= 0);
        if (n == 0) return {};
        fps result{f[0].inv()};
        int m = 1;
        int len = f.size();
        while (m < n) {
            m = std::min(m * 2, n);
            fps sub(f.begin(), f.begin() + std::min(m, len));
            if (m > len) sub.resize(m);
            fps prod = result * sub;
            prod.resize(m);
            prod[0] = 2 - prod[0];
            for (int i = 1; i < m; i++) prod[i] = -prod[i];
            result *= prod;
            result.resize(m);
        }
        result.resize(n);
        return result;
    }

    // Returns the derivative of `f`.
    // Returns an empty vector if `f` is empty.
    static fps derivative(const fps &f) {
        if (f.empty()) return {};
        fps result(f.begin() + 1, f.end());
        for (std::size_t i = 0; i < result.size(); i++) result[i] *= i + 1;
        return result;
    }

    // Returns the integral of `f`.
    // Sets the `0`-th coefficent of the integral as `0`.
    // If `fps` is empty, returns an empty vector.
    static fps integral(const fps &f) {
        if (f.empty()) return {};
        int len = f.size();
        fps result(len + 1);
        result[1] = 1;
        for (int i = 2, p = mint::mod(); i <= len; i++) {
            result[i] = p / i * -result[p % i];
        }
        for (int i = 0; i < len; i++) result[i + 1] *= f[i];
        return result;
    }

    // Returns the logarithm of `f` up to the first `n` coefficients.
    // Requires `!f.empty()`.
    // Requires `f[0] == 1`.
    // Requires `n >= 0`.
    static fps logarithm(const fps &f, int n) {
        assert(!f.empty());
        assert(f[0] == 1);
        assert(n >= 0);
        if (n == 0) return {};
        fps prod;
        if (int len = f.size(); len <= n) prod = derivative(f);
        else prod = derivative(fps{f.begin(), f.begin() + n});
        prod *= inverse(f, n - 1);
        prod.resize(n - 1);
        fps result = integral(prod);
        result.resize(n);
        return result;
    }

    // Returns the exponential of `f` up to the first `n` coefficients.
    // Requires `f.empty() || fps[0] == 0`.
    // Requires `n >= 0`.
    static fps exponential(const fps &f, int n) {
        assert(f.empty() || f[0] == 0);
        assert(n >= 0);
        if (n == 0) return {};
        fps result{1};
        if (f.empty()) {
            result.resize(n);
            return result;
        }
        int m = 1;
        int len = f.size();
        while (m < n) {
            m = std::min(m * 2, n);
            fps log = -logarithm(result, m);
            for (int i = 0; i < m && i < len; i++) log[i] += f[i];
            log[0]++;
            result *= log;
            result.resize(m);
        }
        return result;
    }

    // Returns `f` raised to the specified `pow` up to the first `n` coefficients.
    // If `pow == 0`, the resulting `fps` is equivalent to `1`.
    // If `pow > 0 && f.empty()`, the resulting `fps` is equivalent to `0`.
    // Requires `pow >= 0`.
    // Requires `n >= 0`.
    static fps power(const fps &f, int64_t pow, int n) {
        assert(pow >= 0);
        assert(n >= 0);
        if (n == 0) return {};
        if (pow == 0) {
            fps result(n);
            result[0] = 1;
            return result;
        }
        int d = -1;
        {
            int64_t len = f.size();
            int64_t q = (n + pow - 1) / pow;
            if (q > len) q = len;
            for (int i = 0; i < q; i++) {
                if (f[i] == 0) continue;
                d = i;
                break;
            }
            if (d == -1) return fps(n);
        }
        int shift = d * pow;
        int len = f.size() - d;
        if (len > n - shift) len = n - shift;
        fps g(f.begin() + d, f.begin() + d + len);
        mint acc = g[0].pow(pow);
        g = logarithm(g / g[0], n - shift) * pow;
        g = exponential(g, n - shift) * acc;
        fps result(n);
        for (int i = 0; i + shift < n; i++) result[i + shift] = g[i];
        return result;
    }

    // Computes the coefficient of the term with the specified degree `k` in the rational function.
    // Requires `!denominator.empty() && denominator[0] != 0`.
    // Requires `k >= 0`.
    // Reference: https://info.atcoder.jp/entry/algorithm_lectures/linearly_recurrent_sequence_kth_term
    static mint bostan_mori(fps numerator, fps denominator, int64_t k) {
        assert(!denominator.empty() && denominator[0] != 0);
        assert(k >= 0);
        if (numerator.empty()) return 0;
        while (k) {
            fps denom_neg = denominator;
            for (std::size_t i = 1; i < denom_neg.size(); i += 2) {
                denom_neg[i] = -denom_neg[i];
            }
            numerator *= denom_neg;
            denominator *= denom_neg;
            fps new_num, new_denom;
            new_num.reserve((numerator.size() + 1) >> 1);
            new_denom.reserve((denominator.size() + 1) >> 1);
            for (std::size_t i = k & 1; i < numerator.size(); i += 2) new_num.push_back(numerator[i]);
            for (std::size_t i = 0; i < denominator.size(); i += 2) new_denom.push_back(denominator[i]);
            numerator = new_num;
            denominator = new_denom;
            k >>= 1;
        }
        return numerator[0] / denominator[0];
    }

    // Computes term `a[k]` of a homogeneous linear recurrence `a` of order `n`.
    // More specifically, `recurrence` specifies coefficients `c` in the relation
    // `a[i] = c[0] * a[i - 1] + ... + c[n - 1] * a[i - n]`.
    // Returns `0` if `recurrence` and `init` are empty.
    // Requires `recurrence.size() == init.size()`.
    // Requires `k >= 0`.
    static mint solve_recurrence(const fps &recurrence, const fps &init, int64_t k) {
        assert(recurrence.size() == init.size());
        assert(k >= 0);
        int n = recurrence.size();
        if (n == 0) return 0;
        if (k < n) return init[k];
        fps denominator(n + 1, 1);
        for (int i = 0; i < n; i++) denominator[i + 1] = -recurrence[i];
        fps numerator = denominator * init;
        numerator.resize(n);
        return bostan_mori(numerator, denominator, k);
    }
};

}  // namespace kotone

#endif  // KOTONE_FORMAL_POWER_SERIES_HPP
