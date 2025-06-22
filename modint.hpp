#ifndef MODINT_HPP
#define MODINT_HPP

#include <iostream>

template<int MOD = 998244353>
class modint {
  private:
    int value;

  public:
    modint(int64_t v = 0) {
        value = int(v % MOD);
        if (value < 0) value += MOD;
    }

    modint operator + (const modint &other) const {
        return modint(value + other.value);

    }
    modint operator - (const modint &other) const {
        return modint(value - other.value);

    }
    modint operator * (const modint &other) const {
        return modint(int64_t(value) * other.value);
    }

    modint operator / (const modint &other) const {
        return *this * other.inv();
    }

    modint operator += (const modint &other) {
        value += other.value;
        if (value >= MOD) value -= MOD;
        return *this;
    }

    modint operator -= (const modint &other) {
        value -= other.value;
        if (value < 0) value += MOD;
        return *this;
    }

    modint operator *= (const modint &other) {
        value = int64_t(value) * other.value % MOD;
        return *this;
    }

    modint operator /= (const modint &other) {
        return *this *= other.inv();
    }

    modint pow(int64_t exp) const {
        modint result = 1;
        modint base = *this;
        while (exp > 0) {
            if (exp & 1) result *= base;
            base *= base;
            exp >>= 1;
        }
        return result;
    }

    modint inv() const {
        return this->pow(MOD - 2);
    }

    friend std::ostream &operator << (std::ostream &out, const modint &m) {
        return out << m.value;
    }

    friend std::istream &operator >> (std::istream &in, modint &m) {
        int64_t x;
        in >> x;
        m = modint(x);
        return in;
    }
};

#endif // MODINT_HPP
