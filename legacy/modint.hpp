#ifndef MODINT_HPP
#define MODINT_HPP 1

#include <iostream>

namespace kotone {

template<int MOD = 998244353>
class modint {
    int _val;

    modint(int64_t v = 0) {
        _val = int(v % MOD);
        if (_val < 0) _val += MOD;
    }

    modint operator+(const modint &other) const {
        return modint(_val + other._val);

    }
    modint operator-(const modint &other) const {
        return modint(_val - other._val);

    }
    modint operator*(const modint &other) const {
        return modint(int64_t(_val) * other._val);
    }

    modint operator/(const modint &other) const {
        return *this * other.inv();
    }

    modint operator+=(const modint &other) {
        _val += other._val;
        if (_val >= MOD) _val -= MOD;
        return *this;
    }

    modint operator-=(const modint &other) {
        _val -= other._val;
        if (_val < 0) _val += MOD;
        return *this;
    }

    modint operator*=(const modint &other) {
        _val = int64_t(_val) * other._val % MOD;
        return *this;
    }

    modint operator/=(const modint &other) {
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

    std::ostream& operator<<(std::ostream &out, const modint &m) {
        return out << m._val;
    }

    std::istream& operator>>(std::istream &in, modint &m) {
        int64_t x;
        in >> x;
        m = modint(x);
        return in;
    }
};

}  // namespace kotone

#endif  // MODINT_HPP
