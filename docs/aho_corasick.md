# Aho–Corasick automaton

The Aho–Corasick automaton resembles a trie with additional edges for transitions that would ordinarily "fail" in a trie. More precisely, given a sequence of string patterns, the Aho–Corasick automaton is a collection of states each representing a prefix of some pattern.

Given a state $S$, the transition $\delta(S, c)$ on character $c$ is defined as the longest suffix of $S+c$ present as a state in the automaton.

Thus, if $S+c$ is not a state in the automaton, then $S^\prime$ is obtained by recursively traversing the **suffix link** of $S$ until it reaches the root or a state with transition on $c$. The suffix link of $S$ is the longest proper suffix of $S$ that is also present in the automaton.

The root of the automaton represents an empty string. Transitions from the root on a character that does not appear as a prefix of any pattern lead back to the root.

The `aho_corasick` implementation is flexible and allows user-specified functions to propagate pattern-related information across suffix links. See [Construction](#construction) for more information.

<br>

## Construction

```cpp
template <int ALPHABET_SIZE>
using automaton = std::pair<
    std::vector<std::array<int, ALPHABET_SIZE>>,
    std::vector<int>
>;

using vec_pattern = std::vector<std::vector<int>>;

(1) automaton<ALPHABET_SIZE> ac = construct_automaton<ALPHABET_SIZE>(const vec_pattern &patterns)
(2) automaton<ALPHABET_SIZE> ac = construct_automaton<ALPHABET_SIZE, init, apply, push>(const vec_pattern &patterns)
```

Constructs an automaton for the specified patterns.

Returns a pair containing:

- `first`: a `vector` of `array` representing the trie where state $0$ is the root. `first[i][j]` stores the transition from state $i$ on character $j$.
- `second`: a `vector` for which `second[i]` stores the state corresponding to `patterns[i]`.

- (1) Constructs an automaton for the given alphabet size $|\Sigma|$.
- (2) Constructs an automaton for the given alphabet size $|\Sigma|$, and uses the following functions during construction:
    - Calls `void init(int node)` when adding a new state to the trie.
    - Calls `void apply(int node, int pattern)` when reaching the state corresponding to the pattern.
    - Calls `void push(int node, int suffix_link)` after determining the suffix link of the specified node.

    These callbacks can be used to propagate pattern-related information along suffix links during construction.

### Constraints

- $0\leq c\lt|\Sigma|$ for each character $c$ in a pattern.

### Time complexity

- $\mathcal{O}(M|\Sigma|)$, where $M$ is the total number of characters in `patterns`, and $|\Sigma|$ is the size of the alphabet.

<br>

## Related problems (external links)

<details><summary>Click to unfold (spoilers)</summary>

- [ABC 268 Ex - Taboo](https://atcoder.jp/contests/abc268/tasks/abc268_h)
- [ABC 305 G - Banned Substrings](https://atcoder.jp/contests/abc305/tasks/abc305_g)
- [ABC 362 G - Count Substring Query](https://atcoder.jp/contests/abc362/tasks/abc362_g)
- [ABC 419 F - All Included](https://atcoder.jp/contests/abc419/tasks/abc419_f)
- [ABC 458 F - Critical Misread](https://atcoder.jp/contests/abc458/tasks/abc458_f)
- [ARC 141 F - Well-defined Abbreviation](https://atcoder.jp/contests/arc141/tasks/arc141_f)

</details>

<br>
