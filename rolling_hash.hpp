#ifndef ROLLING_HASH_HPP
#define ROLLING_HASH_HPP 1

#include <string>
#include <atcoder/modint>

using mint = atcoder::modint998244353;

struct static_string_hash {
    mint _hash991, _pow991, _hash997, _pow997;

    static_string_hash()
        : _hash991(0), _pow991(1), _hash997(0), _pow997(1) {}

    static_string_hash(char c) : static_string_hash() {
        join(c);
    }

    static_string_hash(const std::string &s) : static_string_hash() {
        join(s);
    }

    static_string_hash(const static_string_hash &prefix, const static_string_hash &suffix) {
        _hash991 = prefix._hash991;
        _pow991 = prefix._pow991;
        _hash997 = prefix._hash997;
        _pow997 = prefix._pow997;
        join(suffix);
    }

    void join(char c) {
        _hash991 = _hash991 * 991 + c;
        _pow991 *= 991;
        _hash997 = _hash997 * 997 + c;
        _pow997 *= 997;
    }

    void join(const std::string &s) {
        for (char c : s) {
            _hash991 = _hash991 * 991 + c;
            _pow991 *= 991;
            _hash997 = _hash997 * 997 + c;
            _pow997 *= 997;
        }
    }

    void join(const static_string_hash &other) {
        _hash991 = _hash991 * other._pow991 + other._hash991;
        _pow991 *= other._pow991;
        _hash997 = _hash997 * other._pow997 + other._hash997;
        _pow997 *= other._pow997;
    }

    bool operator==(const static_string_hash &other) const {
        return (
            _hash991 == other._hash991
            && _pow991 == other._pow991
            && _hash997 == other._hash997
            && _pow997 == other._pow997
        );
    }

    bool operator<(const static_string_hash &other) const {
        if (_hash991 != other._hash991) return _hash991.val() < other._hash991.val();
        if (_pow991 != other._pow991) return _pow991.val() < other._pow991.val();
        if (_hash997 != other._hash997) return _hash997.val() < other._hash997.val();
        return _pow997.val() < other._pow997.val();
    }
};

// Reference:
// https://stackoverflow.com/questions/20511347/a-good-hash-function-for-a-vector/72073933#72073933

struct size_t_hash {
    std::size_t operator()(const static_string_hash &h) const {
        std::size_t hash = 0;
        for (mint m : {h._hash991, h._pow991, h._hash997, h._pow997}) {
            uint32_t x = m.val();
            x = (x >> 16 ^ x) * 0x45d9f3bu;
            x = (x >> 16 ^ x) * 0x45d9f3bu;
            x = x >> 16 ^ x;
            hash ^= x + 0x9e3779b9u + (hash << 6) + (hash >> 2);
        }
        return hash;
    }
};

#endif  // ROLLING_HASH_HPP
