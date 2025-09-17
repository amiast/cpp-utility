# Mo's algorithm

Mo's algorithm is a technique for efficiently answering two-dimensional offline queries.

For instance, Mo's algorithm can efficiently answer range queries for a sequence.

> Given a sequence $A=(A_0, \dots, A_{N-1})$ of $N$ elements, let $A_{\ell, r}=(A_\ell, \dots, A_{r-1})$ be the contiguous subsequence in the half-open interval $[\ell, r)$ for $0\leq\ell\leq r\leq N$. Note that $[\ell, r)$ is empty when $\ell=r$.
>
> Given a function $f$ that maps contiguous subsequences of $A$ to some values, a range query to evaluate $f(A_{\ell, r})$ can be expressed as the half-open interval $[\ell, r)$.

Instead of processing each query individually, it may be more efficient to modify previous results when computing new queries. More precisely, the result to a new range query can be obtained by a combination of the following transformations:

* $\text{ADD}_L(f(A_{\ell, r}))=f(A_{\ell-1, r})$
* $\text{ADD}_R(f(A_{\ell, r}))=f(A_{\ell, r+1})$
* $\text{DEL}_L(f(A_{\ell, r}))=f(A_{\ell+1, r})$
* $\text{DEL}_R(f(A_{\ell, r}))=f(A_{\ell, r-1})$

Mo's algorithm stores the current range query $[\ell, r)$ and its result $f(A_{\ell, r})$ during each step of computation. To answer a new query $[\ell^\prime, r^\prime)$, the algorithm iteratively expands or contracts the interval via $\text{ADD}$ and $\text{DEL}$ operations to "add" and "delete" elements at the start and end of the subsequence. For example, to evaluate $[\ell, r+10)$ after evaluating $[\ell, r)$, the algorithm iteratively applies $\text{ADD}_R$ to compute $[\ell, r+1), \dots, [\ell, r+10)$. Likewise, to compute $[\ell+10, r)$ from $[\ell, r)$, the algorithm contracts the interval via $\text{DEL}_L$ and computes $[\ell+1, r), \dots, [\ell+10, r)$.

In general, Mo's algorithm works for offline queries that can be described by two non-negative integer parameters $(x, y)$. This implementation follows the convention of using $[\ell, r)$ to represent range queries, but it also works for non-range queries where $\ell\gt r$.

The original Mo's algorithm uses square-root decomposition to determine the order of query evaluation. A faster method uses the Hilbert curve as a heuristic to minimize the total number of evaluations needed to answer all queries.

The implementation is based on [Takeshi Masumoto's Algorithm Book](https://take44444.github.io/Algorithm-Book/range/mo/main.html) (in Japanese).

<br>

## Constructor

```cpp
kotone::mo_alg mo()
```

Constructs an empty instance to store range queries.

### Time complexity

* $\mathcal{O}(1)$

<br>

## Insert queries

```cpp
int mo.insert_query(int l, int r)
```

Inserts range query $[\ell, r)$ and returns its query index.

### Constraints

* $\ell\geq0$
* $r\geq0$
* For range queries, $0\leq\ell\leq r\leq N$

### Time complexity

* $\mathcal{O}(1)$

<br>

## Evaluate queries

```cpp
(1) void mo.eval_queries(order_ &&order, add_l_ &&add_l, add_r_ && add_r, del_l_ &&del_l, del_r_ &&del_r, solve_ &&solve)
(2) void mo.eval_queries(order_ &&order, add_ &&add, del_ &&del, solve_ &&solve)
```

Evaluates range queries using the provided functions.

* The algorithm assumes $[0, 0)$ as the initial state.
* `T order(int l, int r)` returns some value of type `T` that can be sorted with respect to the `<` operator. It is used for optimizing query order.
* (1) `void add_l(int l, int r)` modifies the result of range query $[\ell+1, r)$ to evaluate $[\ell, r)$, thereby "adding" $A_\ell$ to the start of the subsequence.
* (1) `void add_r(int l, int r)` modifies the result of range query $[\ell, r)$ to evaluate $[\ell, r+1)$, thereby "adding" $A_r$ to the end of the subsequence.
* (1) `void del_l(int l, int r)` modifies the result of range query $[\ell, r)$ to evaluate $[\ell+1, r)$, thereby "deleting" $A_\ell$ from the start of the subsequence.
* (1) `void del_r(int l, int r)` modifies the result of range query $[\ell, r+1)$ to evaluate $[\ell, r)$, thereby "deleting" $A_r$ from the end of the subsequence.
* (2) `void add(int i)` is a specialized version of `add_l` and `add_r` that depends on only the modified dimension. It is equivalent to `add_l(i, j)` on $[i + 1, j)$ and `add_r(j, i)` on $[j, i)$ for arbitrary $j$, thereby "adding" $A_i$ to the appropriate end of the subsequence.
* (2) `void del(int i)` is a specialized version of `del_l` and `del_r` that depends on only the modified dimension. It is equivalent to `del_l(i, j)` on $[i, j)$ and `del_r(j, i)` on $[j, i+1)$ for arbitrary $j$, thereby "deleting" $A_i$ from the appropriate end of the subsequence.
* `void solve(int i)` evaluates the `i`-th query.

### Time complexity

* $\mathcal{O}(Q(\alpha+\beta+\log Q)+\gamma(N + Q)\sqrt{N})$, where $Q$ is the number of queries, $\alpha$ is the computational cost of each `order` call, $\beta$ is the computational cost of each `solve` call, and $\gamma$ is the computational cost of each $\text{ADD}$ and $\text{DEL}$ call, assuming an appropriate `order` function based on square-root decomposition or Hilbert curve.

<br>

## Hilbert index

```cpp
int64_t hilbert_index<int max_bit_width = 30>(int x, int y)
```

Returns the index of the coordinates $(x, y)$ on a Hilbert curve given the maximum bit width $B_{\max}$.

* Pass this function to `mo_alg::eval_queries` to determine an optimized query order.
* Example usage: `mo.eval_queries(kotone::hilbert_index<>, add, del, solve)`

### Constraints

* $0\lt B_{\max}\leq30$
* $0\leq x, y\lt 2^{B_{\max}}$

### Time complexity

* $\mathcal{O}(B_{\max})$

<br>

## Sample code

The sample code is a solution to the following problem:

<details><summary>Click to unfold (spoilers)</summary>

* [ABC 242 G - Range Pairing Query](https://atcoder.jp/contests/abc242/tasks/abc242_g)

</details>

<br>

```cpp
#include <iostream>
#include <vector>
#include <kotone/mo_algorithm>

int main() {
    int N;
    std::cin >> N;
    std::vector<int> A(N);
    for (int &a : A) std::cin >> a;

    int Q;
    std::cin >> Q;
    kotone::mo_alg mo;
    for (int i = 0, l, r; i < Q; i++) {
        std::cin >> l >> r;
        mo.insert_query(l - 1, r);
    }

    std::vector<int> count(N + 1), result(Q);
    int total_count = 0;
    auto add = [&](int i) { if (++count[A[i]] % 2 == 0) total_count++; };
    auto del = [&](int i) { if (--count[A[i]] % 2 == 1) total_count--; };
    auto solve = [&](int i) { result[i] = total_count; };
    mo.eval_queries(kotone::hilbert_index<17>, add, del, solve);

    for (int r : result) std::cout << r << std::endl;
}
```

<br>

## Related problems (external links)

<details><summary>Click to unfold (spoilers)</summary>

* [ABC 242 G - Range Pairing Query](https://atcoder.jp/contests/abc242/tasks/abc242_g)
* [ABC 293 G - Triple Index](https://atcoder.jp/contests/abc293/tasks/abc293_g)
* [ABC 384 G - Abs Sum](https://atcoder.jp/contests/abc384/tasks/abc384_g)
* [ABC 405 G - Range Shuffle Query](https://atcoder.jp/contests/abc405/tasks/abc405_g)

</details>

<br>
