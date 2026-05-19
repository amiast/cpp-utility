#ifndef KOTONE_MODINT_UTILITY_HPP
#define KOTONE_MODINT_UTILITY_HPP 1

#include <vector>
#include <cassert>
#include <kotone/internal_type_traits>

namespace kotone {

// Returns a length-`(n+1)` vector `reciprocal` such that `reciprocal[i]` is the modular inverse of `i`.
// The value of `reciprocal[0]` is undefined.
// Requires `mint::mod()` to be prime.
// Requires `1 <= n < mint::mod()`.
template <compatible_modint mint> std::vector<mint> reciprocals(int n) {
    int p = mint::mod();
    assert(1 <= n && n <= p);
    std::vector<mint> result(n + 1);
    result[1] = 1;
    for (int i = 2; i <= n; i++) {
        result[i] = -(p / i) * result[p % i];
    }
    return result;
}

// Returns a vector containing the first `n + 1` factorials (`0!, 1!, ..., n!`).
// Requires `n >= 0`.
template <compatible_modint mint> std::vector<mint> factorials(int n) {
    assert(n >= 0);
    std::vector<mint> result(n + 1);
    result[0] = 1;
    for (int i = 1; i <= n; i++) result[i] = i * result[i - 1];
    return result;
}

// Returns a vector of inverse factorials given the vector of factorials.
// Requires `vec_factorial` to match the format of the output of `factorials()`.
// Requires `mint::mod()` to be prime.
// Requires `1 <= vec_factorial.size() <= mint::mod()`.
template <compatible_modint mint> std::vector<mint> inv_factorials(const std::vector<mint> &vec_factorial) {
    int len = vec_factorial.size();
    int p = mint::mod();
    assert(1 <= len && len <= p);
    std::vector<mint> result(len);
    assert(vec_factorial[len - 1] != 0);
    result[len - 1] = vec_factorial[len - 1].inv();
    for (int i = len - 1; i > 0; i--) result[i - 1] = i * result[i];
    return result;
}

// A wrapper class for combinatorial functions with modint.
template <compatible_modint mint> struct modint_utility {
  private:
    std::vector<mint> _inv, _fact, _ifact;
    int _p = -1, _max_n = 0;

    void _build(int new_max) {
        if (new_max <= _max_n) return;

        _inv.resize(new_max + 1, 1);
        _fact.resize(new_max + 1, 1);
        _ifact.resize(new_max + 1, 1);
        for (int i = std::max(_max_n, 2); i <= new_max; i++) {
            _inv[i] = -(_p / i) * _inv[_p % i];
        }
        for (int i = std::max(_max_n, 2); i < new_max; i++) _fact[i] = i * _fact[i - 1];
        for (int i = std::max(_max_n, 2); i < new_max; i++) _ifact[i] = _inv[i] * _ifact[i - 1];
        _max_n = new_max;
    }

  public:
    modint_utility() {}

    // Instantiates with the first `n + 1` factorials (`0!, 1!, ..., n!`) precomputed.
    // Requires `mint::mod()` to be prime.
    // Requires `mint::mod()` to be invariant.
    // Requires `n < mint::mod()`.
    modint_utility(int n) {
        _p = mint::mod();
        assert(n < _p);
        _build(n);
    }

    // Returns the modular inverse of `n`.
    // Requires `1 <= n < mint::mod()`.
    mint inv(int n) {
        assert(1 <= n && n < _p);
        if (n >= _max_n) _build(n);
        return _inv[n];
    }

    // Returns `n!`.
    // Requires `0 <= n < mint::mod()`.
    mint factorial(int n) {
        assert(0 <= n && n < _p);
        if (n >= _max_n) _build(n);
        return _fact[n];
    }

    // Returns the modular inverse of `n!`.
    // Requires `0 <= n < mint::mod()`.
    mint inv_factorial(int n) {
        assert(0 <= n && n < _p);
        if (n >= _max_n) _build(n);
        return _ifact[n];
    }

    // Returns the number of permutations of `k` of `n` objects.
    // Requires `0 <= k <= n < mint::mod()`.
    mint perm(int n, int k) {
        assert(0 <= k && k <= n && n < _p);
        if (n >= _max_n) _build(n);
        return _fact[n] * _ifact[n - k];
    }

    // Returns the number of combinations of `k` of `n` objects.
    // Requires `0 <= k <= n < mint::mod()`.
    mint comb(int n, int k) {
        assert(0 <= k && k <= n && n < _p);
        if (n >= _max_n) _build(n);
        return _fact[n] * _ifact[k] * _ifact[n - k];
    }

    // Returns the `n`-th Catalan number.
    // Requires `0 <= n * 2 < _p`.
    mint catalan(int n) {
        assert(0 <= n && n * 2 < _p);
        if (n * 2 >= _max_n) _build(n * 2);
        return _fact[n * 2] * _ifact[n + 1] * _ifact[n];
    }
};

// Returns a square root of the given value.
// If no square root exists, returns `0`.
// If there are multiple square roots, the function may return any of them.
template <compatible_modint mint> mint sqrt_mint(mint val) {
    if (val == 0) return 0;
    int mod = mint::mod();
    if (mod == 2) return val;
    if (val.pow((mod - 1) / 2) == -1) return 0;
    if (mod % 4 == 3) return val.pow((mod + 1) / 4);

    int s = 0;
    int q = mod - 1;
    while (q % 2 == 0) {
        q /= 2;
        s++;
    }

    mint z = 2;
    while (z.pow((mod - 1) / 2) == 1) z++;
    mint c = z.pow(q);
    mint x = val.pow((q + 1) / 2);
    mint t = val.pow(q);

    while (t != 1) {
        int i = 0;
        mint tpow = t;
        while (tpow != 1 && i < s) {
            tpow *= tpow;
            i++;
        }
        if (i == s) return 0;
        mint b = c.pow(1 << (s - i - 1));
        x *= b;
        t *= b * b;
        c = b * b;
        s = i;
    }
    return x;
}

}  // namespace kotone

#endif  // KOTONE_MODINT_UTILITY_HPP
