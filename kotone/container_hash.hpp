#ifndef CONTAINER_HASH_HPP
#define CONTAINER_HASH_HPP 1

#include <cstdint>
#include <vector>
#include <array>

namespace kotone {

// Reference:
// https://stackoverflow.com/questions/20511347/a-good-hash-function-for-a-vector/72073933#72073933
template <typename T>
struct vector_hash {
    std::size_t operator()(const std::vector<T> &v) const {
        std::size_t hash = 0;
        for (T val : v) {
            uint32_t x = val >> 32 ^ val;
            x = (x >> 16 ^ x) * 0x45d9f3bu;
            x = (x >> 16 ^ x) * 0x45d9f3bu;
            x = x >> 16 ^ x;
            hash ^= x + 0x9e3779b9u + (hash << 6) + (hash >> 2);
        }
        return hash;
    }
};

template <typename T, std::size_t N>
struct array_hash {
    std::size_t operator()(const std::array<T, N> &v) const {
        std::size_t hash = 0;
        for (T val : v) {
            uint32_t x = val >> 32 ^ val;
            x = (x >> 16 ^ x) * 0x45d9f3bu;
            x = (x >> 16 ^ x) * 0x45d9f3bu;
            x = x >> 16 ^ x;
            hash ^= x + 0x9e3779b9u + (hash << 6) + (hash >> 2);
        }
        return hash;
    }
};

}  // namespace kotone

#endif  // CONTAINER_HASH_HPP
