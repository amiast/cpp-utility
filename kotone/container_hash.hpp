#ifndef KOTONE_CONTAINER_HASH_HPP
#define KOTONE_CONTAINER_HASH_HPP 1

#include <cstdint>
#include <vector>
#include <array>
#include <utility>
#include <concepts>

namespace kotone {

// A hash for vectors of integral values.
// Reference: https://stackoverflow.com/questions/20511347/a-good-hash-function-for-a-vector/72073933#72073933
template <std::integral T>
struct vector_hash {
    std::size_t operator()(const std::vector<T> &v) const {
        std::size_t hash = 0;
        for (T val : v) {
            uint32_t x = static_cast<uint32_t>(static_cast<uint64_t>(val) >> 32 ^ val);
            x = (x >> 16 ^ x) * 0x45d9f3bu;
            x = (x >> 16 ^ x) * 0x45d9f3bu;
            x = x >> 16 ^ x;
            hash ^= x + 0x9e3779b9u + (hash << 6) + (hash >> 2);
        }
        return hash;
    }
};


// A hash for arrays of integral values.
// Reference: https://stackoverflow.com/questions/20511347/a-good-hash-function-for-a-vector/72073933#72073933
template <std::integral T, std::size_t N>
struct array_hash {
    std::size_t operator()(const std::array<T, N> &v) const {
        std::size_t hash = 0;
        for (T val : v) {
            uint32_t x = static_cast<uint32_t>(static_cast<uint64_t>(val) >> 32 ^ val);
            x = (x >> 16 ^ x) * 0x45d9f3bu;
            x = (x >> 16 ^ x) * 0x45d9f3bu;
            x = x >> 16 ^ x;
            hash ^= x + 0x9e3779b9u + (hash << 6) + (hash >> 2);
        }
        return hash;
    }
};

// A hash for pairs of integral values.
// If possible, consider hashing plain integers as a computationally-cheaper alternative.
template <std::integral S, std::integral T>
struct pair_hash {
    std::size_t operator()(const std::pair<S, T> &p) const {
        uint64_t x = static_cast<uint64_t>(p.first);
        uint64_t y = static_cast<uint64_t>(p.second);
        return std::hash<uint64_t>()((x >> 32 ^ x) << 32 | (y >> 32 ^ y));
    }
};

}  // namespace kotone

#endif  // KOTONE_CONTAINER_HASH_HPP
