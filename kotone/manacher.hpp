#ifndef KOTONE_MANACHER_HPP
#define KOTONE_MANACHER_HPP 1

#include <string>
#include <vector>

namespace kotone {

// Computes `t = '~'.join(s)` and returns a pair containing:
// - `first`: string `t`
// - `second`: a vector storing the radius of the longest palindrome centered at each position in `t`.
//
// Reference: https://cp-algorithms.com/string/manacher.html
std::pair<std::string, std::vector<int>> manacher(const std::string &s) {
    std::string t = "$";
    int m = s.size();
    for (int i = 0; i < m - 1; i++) t += s[i], t += '~';
    t += s[m - 1];
    t += '^';
    int n = t.size();
    std::vector<int> vec(n);
    int l = 0, r = 1;
    for (int i = 1; i < n - 1; i++) {
        if (i <= r) vec[i] = std::min(r - i, vec[l + r - i]);
        while (t[i - vec[i]] == t[i + vec[i]]) vec[i]++;
        if (i + vec[i] > r) {
            l = i - vec[i];
            r = i + vec[i];
        }
    }
    return {{t.begin() + 1, t.end() - 1}, {vec.begin() + 1, vec.end() - 1}};
}

}  // namespace kotone

#endif  // KOTONE_MANACHER_HPP
