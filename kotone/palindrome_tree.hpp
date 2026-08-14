#ifndef KOTONE_PALINDROME_TREE_HPP
#define KOTONE_PALINDROME_TREE_HPP 1

#include <vector>
#include <array>
#include <cassert>

namespace kotone {

// A node in a palindrome tree.
// Includes the following attributes:
// - `size`: the length of the palindrome represented by the node.
// - `link`: the node containing the longest proper palindromic suffix. If `size <= 0`, then `link == 0`.
// - `parent`: the parent node. If `size <= 0`, then `parent == -1`.
// - `children`: the array of children. Uses `-1` for children that do not exist.
//
// Requires `ALPHABET_SIZE >= 0`.
template <int ALPHABET_SIZE> struct palindrome_tree_node {
    static_assert(ALPHABET_SIZE >= 0);
    int size = 0, link = 0, parent = -1;
    std::array<int, ALPHABET_SIZE> children;
    palindrome_tree_node() { children.fill(-1); }
    palindrome_tree_node(int s) : size(s) { children.fill(-1); }
};

// Returns the palindrome tree of the given string.
// Optionally, pass the following functions:
// - `void init(int index, int size)`
// - `void on_push(int pos, int index_longest_suffix)`
//
// Requires `ALPHABET_SIZE >= 0`.
// Requires `0 <= str[i] < ALPHABET_SIZE` for all `i`.
// Reference: https://judge.yosupo.jp/problem/eertree
template <int ALPHABET_SIZE, void (*init)(int, int) = nullptr, void (*on_push)(int, int) = nullptr>
std::vector<palindrome_tree_node<ALPHABET_SIZE>> palindrome_tree(const std::vector<int> &str) {
    int len = str.size();
    for (int i = 0; i < len; i++) assert(0 <= str[i] && str[i] < ALPHABET_SIZE);
    using node = palindrome_tree_node<ALPHABET_SIZE>;
    std::vector<node> tree;
    auto link = [&](int i, int k) {
        while (true) {
            int j = k - 1 - tree[i].size;
            if (j >= 0 && str[j] == str[k]) break;
            i = tree[i].link;
        }
        return i;
    };
    for (int i = 0; i < 2; i++) {
        if constexpr (init) init(i, i - 1);
        tree.emplace_back(i - 1);
    }
    int prev = 1;
    for (int k = 0; k < len; k++) {
        int c = str[k];
        int i = link(prev, k);
        if (tree[i].children[c] == -1) {
            int ni = tree.size();
            if constexpr(init) init(ni, tree[i].size + 2);
            tree.emplace_back(tree[i].size + 2);
            node &n = tree.back();
            n.parent = i;
            int j = link(tree[i].link, k);
            if (int sl = tree[j].children[c]; sl >= 0) n.link = sl;
            else n.link = 1;
            tree[i].children[c] = ni;
        }
        prev = tree[i].children[c];
        if constexpr (on_push) on_push(k, prev);
    }
    return tree;
}

}  // namespace kotone

#endif  // KOTONE_PALINDROME_TRE_HPP
