# Mo's algorithm

Mo's algorithm is a technique for efficiently answering two-dimensional queries.

For instance, Mo's algorithm can efficiently answer range queries on a sequence.

> Given a sequence $A=(A_0, \dots, A_{N-1})$ of $N$ elements, let $A_{\ell, r}=(A_\ell, \dots, A_{r-1})$ be the contiguous subsequence in the half-open interval $[\ell, r)$ for $0\leq\ell\leq r\leq N$. Note that $[\ell, r)$ is an empty sequence when $\ell=r$.
>
> Given an arbitrary function $f$ that maps contiguous subsequences of $A$ to some values, a range query to evaluate $f(A_{\ell, r})$ can be expressed as the half-open interval $[\ell, r)$.

In general, Mo's algorithm works for queries that can be described by two non-negative integer parameters $(x, y)$. This implementation follows the convention of using $[\ell, r)$ to represent range queries, but it also works for non-range queries where $\ell\gt r$.

Mo's algorithm initially stores the result to the empty interval $[0, 0)$. To answer a new query $[\ell^\prime, r^\prime)$ after evaluating $[\ell, r)$, the algorithm iteratively expands or contracts the interval by "adding" and "deleting" elements at the start and end of a subsequence. For example, to evaluate $[\ell, r+10)$ after evaluating $[\ell, r)$, the algorithm uses existing results to iteratively expand the interval and compute $[\ell, r+1), \dots, [\ell, r+10)$. Likewise, to compute $[\ell+10, r)$ from $[\ell, r)$, the algorithm contracts the interval and computes $[\ell+1, r), \dots, [\ell+10, r)$.

The original Mo's algorithm uses square-root decomposition to determine the order of query evaluation. A faster method uses Hilbert's curve to minimize the total number of evaluations needed to answer all queries.

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

* `T order(int l, int r)` returns some value of type `T` that can be sorted with respect to the `<` operator. It is used for optimizing query order.
* (1) `void add_l(int l, int r)` modifies the result of range query $[\ell+1, r)$ to evaluate $[\ell, r)$.
* (1) `void add_r(int l, int r)` modifies the result of range query $[\ell, r)$ to evaluate $[\ell, r+1)$.
* (1) `void del_l(int l, int r)` modifies the result of range query $[\ell, r)$ to evaluate $[\ell+1, r)$.
* (1) `void del_r(int l, int r)` modifies the result of range query $[\ell, r+1)$ to evaluate $[\ell, r)$.
* (2) `void add(int x)` is a specialized version of `add_l` and `add_r` that depends on only the modified dimension. It is equivalent to `add_l(x, y)` on $[x + 1, y)$ and `add_r(y, x)` on $[y, x)$ for arbitrary $y$.
* (2) `void del(int x)` is a specialized version of `del_l` and `del_r` that depends on only the modified dimension. It is equivalent to `del_l(x, y)` on $[x, y)$ and `del_r(y, x)` on $[y, x+1)$ for arbitrary $y$.
* `void solve(int i)` evaluates the `i`-th query.

### Time complexity

* $\mathcal{O}(Q(\alpha+\beta+\log Q)+\gamma(N + Q)\sqrt{N})$, where $Q$ is the number of queries, $\alpha$ is the computational cost of each `order` call, $\beta$ is the computational cost of each `solve` call, and $\gamma$ is the computational cost of each "add" and "del" call, assuming an appropriate `order` function based on square-root decomposition or Hilbert curve.

<br>

## Hilbert index

```cpp
template <int max_bit_width = 30> int64_t hilbert_index(int x, int y)
```

Returns the index of the coordinates $(x, y)$ on a Hilbert curve given the maximum bit width $B_{\max}$.

* Pass this function to `mo_alg::eval_queries` to determine an optimized query order.
* For example: `mo.eval_queries(kotone::hilbert_index<>, add, del, solve)`

### Constraints

* $0\lt B_{\max}\leq30$
* $0\leq x, y\lt 2^{B_{\max}}$

### Time complexity

* $\mathcal{O}(B_{\max})$

<br>

## Related problems (external links)

<details><summary>Click to unfold (spoilers)</summary>

* [ABC 242 G - Range Pairing Query](https://atcoder.jp/contests/abc242/tasks/abc242_g)
* [ABC 293 G - Triple Index](https://atcoder.jp/contests/abc293/tasks/abc293_g)
* [ABC 384 G - Abs Sum](https://atcoder.jp/contests/abc384/tasks/abc384_g)
* [ABC 405 G - Range Shuffle Query](https://atcoder.jp/contests/abc405/tasks/abc405_g)

</details>

<br>
