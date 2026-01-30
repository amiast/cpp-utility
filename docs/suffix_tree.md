# Suffix Tree

The `suffix_tree` is a data structure that implicitly represents a trie for the set of suffixes of a string or sequence.

Given a string $S$ of length $N$, there are $N$ non-empty suffixes of $S$ with a combined length of $\displaystyle\frac{N(N+1)}2$. Thus, the trie for the set of suffixes of $S$ requires $\Theta(N^2)$ nodes in the worst case. On the other hand, the suffix tree compresses suffixes with common prefixes to represent the same trie with $\Theta(N)$ nodes.

This implementation assumes the use of [the Atcoder Library](https://github.com/atcoder/ac-library) for obtaining the suffix array and LCP array of $S$.

This implementation implicitly maintains a monotonic stack over LCP values to build suffix trees in $\Theta(N)$ time. A simpler construction can also be achieved using a segment tree with time complexity $\mathcal{O}(N\log N)$.

Reference: https://www.youtube.com/live/OhRX4kxHsbY?t=10432s

<br>

## Suffix tree node

```cpp
suffix_tree_node {
    int depth, parent, start, stop;
    std::vector<int> children;
};
```

The `suffix_tree_node` is a struct that holds information about a node in a suffix tree. Each node represents a prefix $P$ of some suffixes of $S$. A `suffix_tree_node` object provides the following public attributes:
- `depth`: the length of $P$.
- `parent`: the index of the parent node in the vector returned by `build_suffix_tree`. If this node is the root (and $P$ is the empty string), `parent == -1`.
- `start`: the lexicographic index of the first suffix sharing the prefix $P$.
- `stop`: the lexicographic, past-the-end index of the suffixes sharing the prefix $P$.
- `children`: the vector of indices of the node's children in the vector returned by `build_suffix_tree`. The children are lexicographically ordered by the prefixes they represent.

Note that the lexicographic index of a suffix is the position it appears in the suffix array of $S$.

<br>

## Construction

```cpp
std::vector<suffix_tree_node> build_suffix_tree(const std::vector<int> &sa, const std::vector<int> &lcp)
```

Constructs a suffix tree of $S$ given its suffix array `sa` and LCP array `lcp`.

* Returns a vector consisting of $\Theta(N)$ `suffix_tree_node` objects.
* The first `suffix_tree_node` in the vector is guaranteed to represent the root of the tree.

### Constraints

* `sa` is a valid suffix array and `lcp` is a valid LCP array. For more information, see the [documentation](https://atcoder.github.io/ac-library/production/document_en/string.html) for `<atcoder/string>`.

### Time complexity

* $\Theta(N)$

<br>
