#ifndef KOTONE_MODINT_UTILITY_HPP
#define KOTONE_MODINT_UTILITY_HPP 1

#include <vector>
#include <cassert>
#include <kotone/internal_type_traits>

namespace kotone {

// Returns a length-`(n+1)` vector `reciprocal` such that `reciprocal[i]` is the modular inverse of `i`.
// The value of `reciprocal[0]` is undefined.
// Requires `n >= 1`.
template <compatible_modint mint> std::vector<mint> reciprocals(int n) {
    assert(1 <= n && n <= 100000000);
    std::vector<mint> result(n + 1);
    result[1] = 1;
    int p = mint::mod();
    for (int i = 2; i <= n; i++) {
        result[i] = -(p / i) * result[p % i];
    }
    return result;
}

// Returns a vector containing the first `n + 1` factorials (`0!, 1!, ..., n!`).
// Requires `n >= 0`.
template <compatible_modint mint> std::vector<mint> factorials(int n) {
    assert(0 <= n && n <= 100000000);
    std::vector<mint> result(n + 1);
    result[0] = 1;
    for (int i = 1; i <= n; i++) result[i] = i * result[i - 1];
    return result;
}

// Returns a vector of inverse factorials given the vector of factorials.
// This function should usually be called with `factorials`.
// Requires `vec_factorial` to be a valid vector.
template <compatible_modint mint> std::vector<mint> inv_factorials(const std::vector<mint> &vec_factorial) {
    assert(!vec_factorial.empty());
    int n = vec_factorial.size();
    std::vector<mint> result(n);
    assert(vec_factorial[n - 1] != 0);
    result[n - 1] = vec_factorial[n - 1].inv();
    for (int i = n - 1; i > 0; i--) result[i - 1] = i * result[i];
    return result;
}

// A wrapper class for combinatorial functions with modint.
template <compatible_modint mint> struct modint_utility {
  private:
    int _n = 0;
    std::vector<mint> _fact, _ifact;

    void _build(int new_n) {
        if (new_n <= _n) return;
        assert(new_n <= 100000000);

        if (_n == 0) {
            _fact = factorials<mint>(new_n);
            _ifact = inv_factorials(_fact);
            _n = new_n;
            return;
        }

        _fact.resize(new_n);
        _ifact.resize(new_n);
        for (int i = _n; i < new_n; i++) _fact[i] = i * _fact[i - 1];
        assert(_fact[new_n - 1] != 0);
        _ifact[new_n - 1] = 1 / _fact[new_n - 1];
        for (int i = new_n - 1; i > _n; i--) _ifact[i - 1] = i * _ifact[i];
        _n = new_n;
    }

  public:
    modint_utility() {}

    // Instantiates with the first `n + 1` factorials (`0!, 1!, ..., n!`) precomputed.
    modint_utility(int n) {
        _build(n + 1);
    }

    // Returns `n!`.
    // Requires `n >= 0`.
    mint factorial(int n) {
        assert(n >= 0);
        if (n >= _n) _build(n + 1);
        return _fact[n];
    }

    // Returns the modular inverse of `n!`.
    // Requires `n >= 0`.
    mint inv_factorial(int n) {
        assert(n >= 0);
        if (n >= _n) _build(n + 1);
        return _ifact[n];
    }

    // Returns the number of permutations of `k` of `n` objects.
    // Requires `0 <= k <= n`.
    mint perm(int n, int k) {
        assert(0 <= k && k <= n);
        if (n >= _n) _build(n + 1);
        return _fact[n] * _ifact[n - k];
    }

    // Returns the number of combinations of `k` of `n` objects.
    // Requires `0 <= k <= n`.
    mint comb(int n, int k) {
        assert(0 <= k && k <= n);
        if (n >= _n) _build(n + 1);
        return _fact[n] * _ifact[k] * _ifact[n - k];
    }

    // Returns the `n`-th Catalan number.
    // Requires `n >= 0`.
    mint catalan(int n) {
        assert(n >= 0);
        if (n * 2 >= _n) _build(n * 2 + 1);
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
