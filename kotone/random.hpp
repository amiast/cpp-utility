#ifndef KOTONE_RANDOM_HPP
#define KOTONE_RANDOM_HPP 1

#include <random>
#include <chrono>

namespace kotone {

// Returns a random unsigned 64-bit integer.
uint64_t randint() {
    static std::random_device rd;
    static std::mt19937_64 gen(rd());
    return gen();
}

// A randomized hash for integers.
struct randomized_hash {
  private:
    static uint64_t splitmix64(uint64_t x) noexcept {
        x += 0x9e3779b97f4a7c15;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
        x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
        return x ^ (x >> 31);
    }

  public:
    std::size_t operator()(uint64_t x) const noexcept {
        static const uint64_t SEED = std::chrono::steady_clock::now().time_since_epoch().count();
        return splitmix64(x ^ SEED);
    }
};

}  // namespace kotone

#endif  // KOTONE_RANDOM_HPP
