#ifndef KOTONE_NIMBER_HPP
#define KOTONE_NIMBER_HPP 1

#include <iostream>
#include <array>

namespace kotone {

// An integer-like type encapsulated with nimber arithmetic.
// Reference: https://kyopro-friends.hatenablog.com/entry/2020/04/07/195850
// Reference: https://maspypy.com/library-checker-nim-product
struct nimber {
  private:
    const static inline std::array<std::array<uint8_t, 256>, 256> _small = [](){
        std::array<std::array<uint8_t, 256>, 256> table{};
        table[1][1] = 1;
        for (int k = 1; k < 8; k *= 2) {
            int max = (1 << k) - 1;
            for (int a = 1 << k; a < 1 << (k * 2); a++) {
                int au = a >> k, al = a & max;
                for (int b = 1; b <= a; b++) {
                    int bu = b >> k, bl = b & max;
                    int pu = table[au][bu];
                    int pm = table[au ^ al][bu ^ bl];
                    int pl = table[al][bl];
                    table[a][b] = table[b][a] = (pm ^ pl) << k | (table[pu][1 << (k - 1)] ^ pl);
                }
            }
        }
        return table;
    }();

    constexpr static uint32_t _MAX_U32 = 0xffffffff;
    constexpr static uint16_t _MAX_U16 = 0xffff;
    constexpr static uint8_t _MAX_U8 = 0xff;

    uint64_t _val;

    static uint16_t _prod_16(uint16_t a, uint16_t b) noexcept {
        uint8_t au = a >> 8, al = a & _MAX_U8;
        uint8_t bu = b >> 8, bl = b & _MAX_U8;
        uint8_t pu = _small[au][bu];
        uint8_t pm = _small[au ^ al][bu ^ bl];
        uint8_t pl = _small[al][bl];
        return uint16_t(pm ^ pl) << 8 | (_small[pu][1 << 7] ^ pl);
    }

    static uint16_t _prod_by_15(uint16_t a) noexcept {
        uint8_t au = a >> 8, al = a & _MAX_U8;
        return uint16_t(_small[au ^ al][1 << 7]) << 8 | _small[_small[au][1 << 7]][1 << 7];
    }

    static uint32_t _prod_32(uint32_t a, uint32_t b) noexcept {
        uint16_t au = a >> 16, al = a & _MAX_U16;
        uint16_t bu = b >> 16, bl = b & _MAX_U16;
        uint16_t pu = _prod_16(au, bu);
        uint16_t pm = _prod_16(au ^ al, bu ^ bl);
        uint16_t pl = _prod_16(al, bl);
        return uint32_t(pm ^ pl) << 16 | (_prod_by_15(pu) ^ pl);
    }

    static uint32_t _prod_by_31(uint32_t a) noexcept {
        uint16_t au = a >> 16, al = a & _MAX_U16;
        return uint32_t(_prod_16(au ^ al, 1 << 15)) << 16 | _prod_by_15(_prod_by_15(au));
    }

    static uint64_t _prod_64(uint64_t a, uint64_t b) noexcept {
        uint32_t au = a >> 32, al = a & _MAX_U32;
        uint32_t bu = b >> 32, bl = b & _MAX_U32;
        uint32_t pu = _prod_32(au, bu);
        uint32_t pm = _prod_32(au ^ al, bu ^ bl);
        uint32_t pl = _prod_32(al, bl);
        return uint64_t(pm ^ pl) << 32 | (_prod_by_31(pu) ^ pl);
    }

  public:
    // Constructs nimber equal to `0`.
    nimber() noexcept : _val(0) {}

    // Constructs nimber for the specified value.
    nimber(uint64_t val) noexcept : _val(val) {}

    // Returns the raw value.
    uint64_t val() const noexcept { return _val; }

    nimber& operator+=(const nimber &other) noexcept { _val ^= other._val; return *this; }
    nimber& operator*=(const nimber &other) noexcept { _val = _prod_64(_val, other._val); return *this; }

    friend nimber operator+(const nimber &l, const nimber &r) noexcept { return nimber(l) += r; }
    friend nimber operator*(const nimber &l, const nimber &r) noexcept { return nimber(l) *= r; }

    friend bool operator==(const nimber &l, const nimber &r) noexcept { return l._val == r._val; }
    friend bool operator!=(const nimber &l, const nimber &r) noexcept { return !(l == r); }

    friend std::ostream& operator<<(std::ostream &out, const nimber &n) { return out << n._val; }
    friend std::istream& operator>>(std::istream &in, nimber &n) { return in >> n._val; }
};

}  // namespace kotone

#endif  // KOTONE_NIMBER_HPP
