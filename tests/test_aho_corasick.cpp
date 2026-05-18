// Verified with: https://judge.yosupo.jp/problem/aho_corasick
// Details: https://judge.yosupo.jp/submission/373274
// Contains modifications to the actual implementation.

#include <iostream>
#include <vector>

std::vector<int> parents{-1}, suffix_links;

// #include <kotone/aho_corasick>
// https://github.com/amiast/cpp-utility

#ifndef KOTONE_AHO_CORASICK_HPP
#define KOTONE_AHO_CORASICK_HPP 1

#include <vector>
#include <array>
#include <queue>
#include <cassert>

namespace kotone {

// Constructs an Aho-Corasick automaton for the specified set of patterns,
// then returns a pair containing:
// - `first`: a `vector` of `array` representing the trie, and
// - `second`: a `vector` of the nodes containing each pattern.
//
// Uses node `0` as the root.
//
// Optionally, pass the following functions for state propagation:
// - `void init(int node)`
// - `void apply(int node, int pattern_index)`
// - `void push(int node, int suffix_link)`
//
// Requires `0 <= c < ALPHABET_SIZE` for each character `c` in each pattern.
template <int ALPHABET_SIZE, void (*init)(int) = nullptr, void (*apply)(int, int) = nullptr, void (push)(int, int) = nullptr>
std::pair<std::vector<std::array<int, ALPHABET_SIZE>>, std::vector<int>>
construct_automaton(const std::vector<std::vector<int>> &patterns) {
    std::vector<std::array<int, ALPHABET_SIZE>> trie;
    trie.push_back({});
    if constexpr (init) init(0);
    std::vector<int> matches(patterns.size());

    for (int i = 0; i < int(patterns.size()); i++) {
        int curr = 0;
        for (int c : patterns[i]) {
            assert(0 <= c && c < ALPHABET_SIZE);
            if (trie[curr][c] == 0) {
                trie[curr][c] = trie.size();
                if constexpr (init) init(trie.size());
                trie.push_back({});
                parents.push_back(curr);
            }
            curr = trie[curr][c];
        }
        matches[i] = curr;
        if constexpr (apply) apply(curr, i);
    }

    suffix_links.resize(trie.size());
    std::queue<int> queue;
    for (int n : trie[0]) if (n != 0) queue.push(n);
    while (queue.size()) {
        int curr = queue.front();
        queue.pop();
        if constexpr (push) push(curr, suffix_links[curr]);
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            if (trie[curr][i] == 0) {
                trie[curr][i] = trie[suffix_links[curr]][i];
            } else {
                suffix_links[trie[curr][i]] = trie[suffix_links[curr]][i];
                queue.push(trie[curr][i]);
            }
        }
    }

    return {trie, matches};
}

}  // namespace kotone

#endif  // KOTONE_AHO_CORASICK_HPP

int main() {
    int N;
    std::cin >> N;
    std::vector<std::string> S(N);
    for (std::string &s : S) std::cin >> s;

    std::vector<std::vector<int>> T(N);
    for (int i = 0; i < N; i++) {
        T[i].resize(S[i].size());
        for (unsigned j = 0; j < S[i].size(); j++) {
            T[i][j] = S[i][j] - 'a';
        }
    }

    auto [trie, matches] = kotone::construct_automaton<26>(T);
    std::cout << trie.size() << std::endl;
    for (unsigned i = 1; i < trie.size(); i++) {
        std::cout << parents[i] << ' ' << suffix_links[i] << std::endl;
    }
    for (int m : matches) std::cout << m << ' ';
    std::cout << std::endl;
}
