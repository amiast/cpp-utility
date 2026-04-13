#ifndef KOTONE_WAVELET_MATRIX_HPP
#define KOTONE_WAVELET_MATRIX_HPP 1

#include <vector>
#include <array>
#include <algorithm>
#include <bit>
#include <concepts>
#include <cassert>
#include <atcoder/segtree>

namespace kotone {

// A compact bit vector for `wavelet_matrix`.
// Reference: https://nyaannyaan.github.io/library/data-structure-2d/wavelet-matrix.hpp.html
struct bit_vector {
    static constexpr int _WORDSIZE = 64;
    std::vector<uint64_t> blocks;
    std::vector<int> count;
    int len = 0, zeros = 0;

    bit_vector() {}

    // Constructs a bit vector for the specified length.
    // Values are intialized to `0`.
    bit_vector(int length) {
        assert(length >= 0);
        len = zeros = length;
        blocks.resize(length / _WORDSIZE + 1);
        count.resize(blocks.size());
    }

    // Returns the bit at the specified index.
    bool get(int index) const {
        assert(0 <= index && index < len);
        return blocks[index / _WORDSIZE] >> (index % _WORDSIZE) & 1u;
    }

    // Sets the bit at the specified index.
    void set(int index) {
        assert(0 <= index && index < len);
        blocks[index / _WORDSIZE] |= 1ULL << (index % _WORDSIZE);
    }

    // Resets the bit at the specified index.
    void reset(int index) {
        assert(0 <= index && index < len);
        blocks[index / _WORDSIZE] &= ~(1ULL << (index % _WORDSIZE));
    }

    // Returns the number of bits set to `1` in the specified prefix.
    int rank1(int len_pfx) const {
        assert(0 <= len_pfx && len_pfx <= len);
        return (
            count[len_pfx / _WORDSIZE]
            + std::popcount(blocks[len_pfx / _WORDSIZE] & ((1ULL << (len_pfx % _WORDSIZE)) - 1))
        );
    }

    // Returns the number of bits set to `0` in the specified prefix.
    int rank0(int len_pfx) const {
        return len_pfx - rank1(len_pfx);
    }

    // Build the bit vector after its bits are set.
    void build() {
        for (unsigned i = 1; i < blocks.size(); i++) {
            count[i] = count[i - 1] + std::popcount(blocks[i - 1]);
        }
        zeros = rank0(len);
    }
};

// A static data structure that stores information of a sequence of non-negative integers.
// Reference: https://info.atcoder.jp/entry/algorithm_lectures/wavelet_matrix_basic
template <int BIT_WIDTH> struct wavelet_matrix {
    static_assert(0 <= BIT_WIDTH && BIT_WIDTH <= 64);

  protected:
    std::array<bit_vector, BIT_WIDTH> _mat;
    int _len = 0;

  public:
    wavelet_matrix() {}

    // Constructs a wavelet matrix for the given vector.
    // Requires `0 <= vec[i] < 1 << BIT_WIDTH` for all `i`.
    template <std::integral T> wavelet_matrix(const std::vector<T> &vec) {
        _len = vec.size();
        for (int i = 0; i < _len; i++) assert(vec[i] >= T{});
        if (BIT_WIDTH < 64) {
            uint64_t cap = 1ULL << BIT_WIDTH;
            for (int i = 0; i < _len; i++) assert(uint64_t(vec[i]) < cap);
        }
        std::vector<int> dp(_len);
        for (int i = 0; i < _len; i++) dp[i] = i;
        for (int k = BIT_WIDTH - 1; k >= 0; k--) {
            _mat[k] = bit_vector(_len);
            std::vector<int> ndp(_len);
            for (int i = 0; i < _len; i++) {
                if (vec[dp[i]] >> k & 1) _mat[k].set(i);
            }
            _mat[k].build();
            int left = 0, right = _mat[k].zeros;
            for (int i = 0; i < _len; i++) {
                if (_mat[k].get(i)) ndp[right++] = dp[i];
                else ndp[left++] = dp[i];
            }
            dp = ndp;
        }
    }

    // Returns the `n`-th smallest element in the specified interval `[l, r)`.
    // Requires `0 <= l < r <= _len`.
    // Requires `0 <= n < r - l`.
    uint64_t nth_smallest(int l, int r, int n) const {
        assert(0 <= l && l < r && r <= _len);
        assert(0 <= n && n < r - l);
        uint64_t result = 0;
        for (int k = BIT_WIDTH - 1; k >= 0; k--) {
            int l0 = _mat[k].rank0(l), r0 = _mat[k].rank0(r);
            if (n < r0 - l0) {
                l = l0;
                r = r0;
            } else {
                n -= r0 - l0;
                result |= 1ULL << k;
                l += _mat[k].zeros - l0;
                r += _mat[k].zeros - r0;
            }
        }
        return result;
    }

    // Returns the number of elements less than `upper` in the specified interval `[l, r)`.
    // Requires `0 <= l <= r <= _len`.
    int freq(int l, int r, uint64_t upper) const {
        assert(0 <= l && l <= r && r <= _len);
        if (BIT_WIDTH < 64 && upper >= 1ULL << BIT_WIDTH) return r - l;
        int result = 0;
        for (int k = BIT_WIDTH - 1; k >= 0; k--) {
            bool b = upper >> k & 1u;
            int l0 = _mat[k].rank0(l), r0 = _mat[k].rank0(r);
            if (!b) {
                l = l0;
                r = r0;
            } else {
                result += r0 - l0;
                l += _mat[k].zeros - l0;
                r += _mat[k].zeros - r0;
            }
        }
        return result;
    }

    // Returns a pair `{val, found}` for the maximum value less than `upper` in the specified interval `[l, r)`.
    // If such a value exists, `val` is assigned the value and `found` is true.
    // Otherwise, `found` is `false` (and `val` is assigned `upper`).
    std::pair<uint64_t, bool> find_prev(int l, int r, uint64_t upper) const {
        int count = freq(l, r, upper);
        if (count == 0) return {upper, false};
        return {nth_smallest(l, r, count - 1), true};
    }

    // Returns a pair `{val, found}` for the minimum value at least `lower` in the specified interval `[l, r)`.
    // If such a value exists, `val` is assigned the value and `found` is true.
    // Otherwise, `found` is `false` (and `val` is assigned `lower`).
    std::pair<uint64_t, bool> find_next(int l, int r, uint64_t lower) const {
        int count = freq(l, r, lower);
        if (count == r - l) return {lower, false};
        return {nth_smallest(l, r, count), true};
    }
};

// A static data structure that stores information of a sequence of non-negative integers.
// Modified to also manage commutative monoids with segment trees.
// Reference: https://info.atcoder.jp/entry/algorithm_lectures/wavelet_matrix_basic
// Reference: https://info.atcoder.jp/entry/algorithm_lectures/wavelet_matrix_advanced
template <typename S, S (*op)(S, S), S (*e)(), int BIT_WIDTH> struct wavelet_matrix_commutative : wavelet_matrix<BIT_WIDTH> {
    using wavelet_matrix<BIT_WIDTH>::_mat;
    using wavelet_matrix<BIT_WIDTH>::_len;

  protected:
    std::array<atcoder::segtree<S, op, e>, BIT_WIDTH + 1> _seg;

  public:
    wavelet_matrix_commutative() {}

    // Constructs a wavelet matrix for the given vector and its associated values.
    // Requires `0 <= vec[i] < 1 << BIT_WIDTH` for all `i`.
    template <std::integral T> wavelet_matrix_commutative(const std::vector<T> &vec, const std::vector<S> &vals) {
        _len = vec.size();
        for (int i = 0; i < _len; i++) assert(vec[i] >= T{});
        if (BIT_WIDTH < 64) {
            uint64_t cap = 1ULL << BIT_WIDTH;
            for (int i = 0; i < _len; i++) assert(uint64_t(vec[i]) < cap);
        }
        _seg[BIT_WIDTH] = atcoder::segtree<S, op, e>(vals);
        std::vector<int> dp(_len);
        for (int i = 0; i < _len; i++) dp[i] = i;
        for (int k = BIT_WIDTH - 1; k >= 0; k--) {
            _mat[k] = bit_vector(_len);
            std::vector<int> ndp(_len);
            for (int i = 0; i < _len; i++) {
                if (vec[dp[i]] >> k & 1) _mat[k].set(i);
            }
            _mat[k].build();
            int left = 0, right = _mat[k].zeros;
            for (int i = 0; i < _len; i++) {
                if (_mat[k].get(i)) ndp[right++] = dp[i];
                else ndp[left++] = dp[i];
            }
            dp = ndp;
            std::vector<S> nvals(_len);
            for (int i = 0; i < _len; i++) nvals[i] = vals[dp[i]];
            _seg[k] = atcoder::segtree<S, op, e>(nvals);
        }
    }

    // Constructs a wavelet matrix for the given vector.
    // Requires `0 <= vec[i] < 1 << BIT_WIDTH` for all `i`.
    template <std::integral T> wavelet_matrix_commutative(const std::vector<T> &vec)
        : wavelet_matrix_commutative(vec, std::vector<S>(vec.size(), e())) {}

    // Returns the value associated with the specified index.
    // Requires `0 <= index < _len`.
    S get(int index) const {
        assert(0 <= index && index < _len);
        return _seg[BIT_WIDTH].get(index);
    }

    // Sets the value associated with the specified index.
    // Requires `0 <= index < _len`.
    void set(int index, S val) {
        assert(0 <= index && index < _len);
        _seg[BIT_WIDTH].set(index, val);
        for (int k = BIT_WIDTH - 1; k >= 0; k--) {
            if (!_mat[k].get(index)) index = _mat[k].rank0(index);
            else index = _mat[k].zeros + _mat[k].rank1(index);
            _seg[k].set(index, val);
        }
    }

    // Returns the maximum `upper < 1 << BIT_WIDTH` such that `g(prod(l, r, upper)) == true`.
    // Requires `0 <= l <= r <= _len`.
    // Requires `bool g(S x)` to be a monotonic predicate.
    // Requires `g(e()) == true`.
    template <typename G> uint64_t max_upper(int l, int r, G g) const {
        assert(0 <= l && l <= r && r <= _len);
        assert(g(e()));
        uint64_t result = 0;
        S acc = e();
        for (int k = BIT_WIDTH - 1; k >= 0; k--) {
            int l0 = _mat[k].rank0(l), r0 = _mat[k].rank0(r);
            S new_acc = op(acc, _seg[k].prod(l0, r0));
            if (!g(new_acc)) {
                l = l0;
                r = r0;
            } else {
                result |= 1ULL << k;
                acc = new_acc;
                l += _mat[k].zeros - l0;
                r += _mat[k].zeros - r0;
            }
        }
        return result;
    }

    // Returns the product of values of elements less than `upper` in the specified interval `[l, r)`.
    // Requires `0 <= l <= r <= _len`.
    S prod(int l, int r, uint64_t upper) const {
        assert(0 <= l && l <= r && r <= _len);
        if (BIT_WIDTH < 64 && upper >= 1ULL << BIT_WIDTH) return _seg[BIT_WIDTH].prod(l, r);
        S result = e();
        for (int k = BIT_WIDTH - 1; k >= 0; k--) {
            bool b = upper >> k & 1u;
            int l0 = _mat[k].rank0(l), r0 = _mat[k].rank0(r);
            if (!b) {
                l = l0;
                r = r0;
            } else {
                result = op(result, _seg[k].prod(l0, r0));
                l += _mat[k].zeros - l0;
                r += _mat[k].zeros - r0;
            }
        }
        return result;
    }
};

}  // namespace kotone

#endif  // KOTONE_WAVELET_MATRIX_HPP
