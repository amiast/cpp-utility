#ifndef KOTONE_RANDOM_HPP
#define KOTONE_RANDOM_HPP 1

#include <random>

namespace kotone {

// Returns a random unsigned 64-bit integer.
uint64_t randint() {
    static std::random_device rd;
    static std::mt19937_64 gen(rd());
    return gen();
}

// Reference: https://codeforces.com/blog/entry/62393
uint64_t splitmix64(uint64_t x) noexcept {
    x += 0x9e3779b97f4a7c15;
    x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
    x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
    return x ^ (x >> 31);
}

// A randomized hash for integers.
struct randomized_hash {
    std::size_t operator()(uint64_t x) const {
        static const uint64_t SEED = randint();
        return splitmix64(x ^ SEED);
    }
};

}  // namespace kotone

#endif  // KOTONE_RANDOM_HPP
