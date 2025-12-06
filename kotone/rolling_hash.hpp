#ifndef KOTONE_ROLLING_HASH_HPP
#define KOTONE_ROLLING_HASH_HPP 1

#include <string>
#include <vector>
#include <compare>
#include <cassert>
#include <kotone/internal_type_traits>

namespace kotone {

// A struct that manages the hashes of substrings.
template <compatible_modint mint> struct substring_hash {
  private:
    using hash = std::pair<mint, mint>;
    static inline int _base1 = 991, _base2 = 997;
    static inline std::vector<mint> _pow1, _pow2;
    std::vector<hash> _vec;
    int _size = 0;

    static void _build_pows(int new_size) {
        int old_size = _pow1.size();
        if (old_size >= new_size + 1) return;
        if (old_size) old_size--;
        _pow1.resize(new_size + 1);
        _pow1[0] = 1;
        for (int i = old_size; i < new_size; i++) _pow1[i + 1] = _pow1[i] * _base1;
        _pow2.resize(new_size + 1);
        _pow2[0] = 1;
        for (int i = old_size; i < new_size; i++) _pow2[i + 1] = _pow2[i] * _base2;
    }

  public:
    // Sets the static private attribute `_base1`.
    // This should be called before calling the constructor.
    // Requires `0 < base < mint::mod()`.
    static void set_base1(int base) noexcept {
        assert(0 < base && base < mint::mod());
        _base1 = base;
    }

    // Sets the static private attribute `_base2`.
    // This should be called before calling the constructor.
    // Requires `0 < base < mint::mod()`.
    static void set_base2(int base) noexcept {
        assert(0 < base && base < mint::mod());
        _base2 = base;
    }

    // Prompts the manager to precompute exponents for the given size.
    static void build_pows(int size) {
        _build_pows(size);
    }

    substring_hash() {}

    // Constructs a substring hash manager for the specified string.
    substring_hash(const std::string &s) {
        _size = s.size();
        _vec.resize(_size + 1);
        for (int i = 0; i < _size; i++) {
            _vec[i + 1].first = _vec[i].first * _base1 + s[i];
            _vec[i + 1].second = _vec[i].second * _base2 + s[i];
        }
        _build_pows(_size);
    }

    // Returns the size of the string.
    int size() const noexcept {
        return _size;
    }

    // Returns the hash of the substring in `[l, r)`.
    // Requires `0 <= l <= r <= size()`.
    hash substring(int l, int r) const noexcept {
        assert(0 <= l && l <= r && r <= _size);
        return {_vec[r].first - _vec[l].first * _pow1[r - l], _vec[r].second - _vec[l].second * _pow2[r - l]};
    }

    // Returns the length of the longest common prefix of substrings `[i, size())` and `[j, size())`.
    // Requires `0 <= i <= size()`.
    // Requires `0 <= j <= size()`.
    int lcp(int i, int j) const noexcept {
        assert(0 <= i && i <= _size);
        assert(0 <= j && j <= _size);
        int low = 0;
        int high = std::min(_size - i, _size - j) + 1;
        while (low + 1 < high) {
            int mid = (low + high) / 2;
            if (substring(i, i + mid) == substring(j, j + mid)) low = mid;
            else high = mid;
        }
        return low;
    }
};

// A randomizable rolling hash for strings.
template <compatible_modint mint> struct rolling_hash {
  private:
    static inline int _base1 = 991, _base2 = 997;
    mint _hash1, _pow1, _hash2, _pow2;

  public:
    // Sets the static private attribute `_base1`.
    // This should be called before calling the constructor.
    // Requires `0 < base < mint::mod()`.
    static void set_base1(int base) noexcept {
        assert(0 < base && base < mint::mod());
        _base1 = base;
    }

    // Sets the static private attribute `_base2`.
    // This should be called before calling the constructor.
    // Requires `0 < base < mint::mod()`.
    static void set_base2(int base) noexcept {
        assert(0 < base && base < mint::mod());
        _base2 = base;
    }

    // Instantiates a hash object for an empty string
    rolling_hash() noexcept : _hash1(0), _pow1(1), _hash2(0), _pow2(1) {}

    // Instantiates a hash object for a character.
    rolling_hash(unsigned char c) noexcept : rolling_hash() {
        join(c);
    }

    // Instantiates a hash object for a string.
    rolling_hash(const std::string &s) noexcept : rolling_hash() {
        join(s);
    }

    // Given the hashes of the prefix and suffix strings,
    // instantiates a hash object for their concatenation.
    rolling_hash(const rolling_hash &prefix, const rolling_hash &suffix) noexcept : rolling_hash() {
        join(prefix);
        join(suffix);
    }

    // Updates the hash after inserting a character to the end.
    void join(unsigned char c) noexcept {
        _hash1 = _hash1 * _base1 + c;
        _pow1 *= _base1;
        _hash2 = _hash2 * _base2 + c;
        _pow2 *= _base2;
    }

    // Updates the hash after inserting a string to the end.
    void join(const std::string &s) noexcept {
        for (unsigned char c : s) {
            _hash1 = _hash1 * _base1 + c;
            _pow1 *= _base1;
            _hash2 = _hash2 * _base2 + c;
            _pow2 *= _base2;
        }
    }

    // Updates the hash by concatenation with another string hash.
    void join(const rolling_hash &other) noexcept {
        _hash1 = _hash1 * other._pow1 + other._hash1;
        _pow1 *= other._pow1;
        _hash2 = _hash2 * other._pow2 + other._hash2;
        _pow2 *= other._pow2;
    }

    bool operator==(const rolling_hash &) const noexcept = default;

    auto operator<=>(const rolling_hash &other) const noexcept {
        if (auto cmp = _hash1.val() <=> other._hash1.val(); cmp != 0) return cmp;
        if (auto cmp = _pow1.val() <=> other._pow1.val(); cmp != 0) return cmp;
        if (auto cmp = _hash2.val() <=> other._hash2.val(); cmp != 0) return cmp;
        return _pow2.val() <=> other._pow2.val();
    }

    template <typename> friend struct std::hash;
};

}  // namespace kotone

namespace std {

// Reference: https://stackoverflow.com/questions/20511347/a-good-hash-function-for-a-vector/72073933#72073933
template <typename mint> struct hash<kotone::rolling_hash<mint>> {
    std::size_t operator()(const kotone::rolling_hash<mint> &h) const {
        std::size_t hash = 0;
        for (mint m : {h._hash1, h._pow1, h._hash2, h._pow2}) {
            uint32_t x = m.val();
            x = (x >> 16 ^ x) * 0x45d9f3bu;
            x = (x >> 16 ^ x) * 0x45d9f3bu;
            x = x >> 16 ^ x;
            hash ^= x + 0x9e3779b9u + (hash << 6) + (hash >> 2);
        }
        return hash;
    }
};

}  // namespace std

#endif  // KOTONE_ROLLING_HASH_HPP
