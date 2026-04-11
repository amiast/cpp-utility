#ifndef KOTONE_WAVELET_MATRIX_HPP
#define KOTONE_WAVELET_MATRIX_HPP 1

#include <vector>
#include <array>
#include <algorithm>
#include <bit>
#include <concepts>
#include <cassert>

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

  private:
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

}  // namespace kotone

#endif  // KOTONE_WAVELET_MATRIX_HPP
