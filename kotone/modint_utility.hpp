#ifndef KOTONE_MODINT_UTILITY_HPP
#define KOTONE_MODINT_UTILITY_HPP

#include <vector>
#include <cassert>
#include <kotone/internal_type_traits>

namespace kotone {

// Returns the vector of the first `n` factorials (`0!, 1!, ..., (N-1)!`).
template <compatible_modint mint> std::vector<mint> factorials(int n) {
    assert(n >= 0);
    assert(n <= 100000000);
    std::vector<mint> result(n);
    if (n) result[0] = 1;
    for (int i = 1; i < n; i++) result[i] = i * result[i - 1];
    return result;
}

// Returns the vector of the first `n` inverse factorials.
template <compatible_modint mint> std::vector<mint> inv_factorials(const std::vector<mint> &factorials) {
    int n = static_cast<int>(factorials.size());
    std::vector<mint> result(n);
    if (n) {
        assert(factorials[n - 1] != 0);
        result[n - 1] = 1 / factorials[n - 1];
    }
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

    // Instantiates with the first `n` factorials (`0!, 1!, ..., (n-1)!`) precomputed.
    modint_utility(int n) {
        _build(n);
    }

    mint factorial(int n) {
        assert(n >= 0);
        if (n >= _n) _build(n + 1);
        return _fact[n];
    }

    mint inv_factorial(int n) {
        assert(n >= 0);
        if (n >= _n) _build(n + 1);
        return _ifact[n];
    }

    mint perm(int n, int k) {
        assert(k >= 0);
        assert(n >= k);
        if (n >= _n) _build(n + 1);
        return _fact[n] * _ifact[n - k];
    }

    mint comb(int n, int k) {
        assert(k >= 0);
        assert(n >= k);
        if (n >= _n) _build(n + 1);
        return _fact[n] * _ifact[k] * _ifact[n - k];
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
