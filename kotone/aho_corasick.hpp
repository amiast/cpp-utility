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
// Requires `ALPHABET_SIZE >= 0`.
// Requires `0 <= c < ALPHABET_SIZE` for each character `c` in each pattern.
template <int ALPHABET_SIZE, void (*init)(int) = nullptr, void (*apply)(int, int) = nullptr, void (push)(int, int) = nullptr>
std::pair<std::vector<std::array<int, ALPHABET_SIZE>>, std::vector<int>>
construct_automaton(const std::vector<std::vector<int>> &patterns) {
    static_assert(ALPHABET_SIZE >= 0);
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
            }
            curr = trie[curr][c];
        }
        matches[i] = curr;
        if constexpr (apply) apply(curr, i);
    }

    std::vector<int> suffix_links(trie.size());
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
