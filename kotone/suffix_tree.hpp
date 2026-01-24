#ifndef KOTONE_SUFFIX_TREE_HPP
#define KOTONE_SUFFIX_TREE_HPP 1

#include <vector>

namespace kotone {

// A node in a suffix tree.
// Includes the following attributes:
// - `depth`: the length of the prefix represented by the node.
// - `parent`: the index of the parent node. If this node is the root, `parent == -1`.
// - `start`: the index of the first suffix in the range of the node.
// - `stop`: the index of the past-the-end suffix in the range of the node.
// - `children`: the vector of indices of the node's children in lexicographic order.
struct suffix_tree_node {
    int depth = 0, parent = -1;
    int start = 0, stop = 0;
    std::vector<int> children;
};

// Returns a vector of `suffix_tree_node` representing the (implicit) suffix tree
// given the suffix array (SA) and the LCP array of a string.
// The root is assigned index `0` and represents an empty suffix.
// Requires `sa` and `lcp` to be valid vectors.
std::vector<suffix_tree_node> build_suffix_tree(const std::vector<int> &sa, const std::vector<int> &lcp) {
    std::vector<suffix_tree_node> trie(1);
    int curr = 0;
    int n = sa.size();
    for (int i = 0; i < n; i++) {
        int d = n - sa[i];
        if (trie[curr].depth < d) {
            int child = trie.size();
            trie.emplace_back(d, curr, i);
            trie[curr].children.push_back(child);
            curr = child;
        }
        d = i < n - 1 ? lcp[i] : 0;
        while (d < trie[curr].depth) {
            trie[curr].stop = i + 1;
            curr = trie[curr].parent;
        }
        trie[curr].stop = i + 1;
        if (d == trie[curr].depth) continue;
        int child = trie.size();
        int old_child = trie[curr].children.back();
        trie.emplace_back(d, curr, trie[old_child].start, i + 1);
        trie[child].children.push_back(old_child);
        trie[old_child].parent = child;
        trie[curr].children.back() = child;
        curr = child;
    }
    return trie;
};

}  // namespace kotone

#endif  // KOTONE_SUFFIX_TREE_HPP
