# DSU

The `dsu` is a data structure that maintains disjoint subsets in a partition of a set. This data structure is also known as union-find (UF). This documentation represents subsets of a partition as connected components of an undirected graph. The implementation is based on that of [the Atcoder Library](https://github.com/atcoder/ac-library) with additional maintenance of **potential difference** and **connected component mapping**.

Given an undirected graph $G=(V, E)$ with $N$ nodes labeled $0, \dots, N-1$, a DSU can add edges between nodes and check connectivity in $\mathcal{O}(\alpha(N))$ amortized time per operation. Here, $\alpha$ denotes the inverse Ackermann function. For each connected component of $G$, the DSU assigns a member node as its leader. When the DSU merges two connected components, it reassigns a member node as the new leader.

The potential difference between two connected nodes in $G$ is a function $f:C\rightarrow T$ with the following properties:
* $C\subseteq V^2$ is the set of pairs of connected nodes in $G$
* $T$ is an additive group
* $f(u, v)=-f(v, u)$
* For all simple paths $(v_1, \dots, v_k)$ in $G$, $f(v_1, v_k)=\sum_{i=1}^{k-1}f(v_i, v_{i+1})$

<br>

The `extended_dsu`, derived from `dsu`, maps connected components to user-defined values. When the extended DSU merges two connected components, it merges their images under the mapping as well. More formally, the mapping is a function $g:(\mathcal{P}(V)\setminus\{\varnothing\})\rightarrow S$ with the following properties:
* $S$ is a semigroup equipped with an associative binary operation $h$
* When two disjoint subsets $X, Y$ merge, the mapping obeys $g(X\cup Y)=h(g(X), g(Y))$

<br>

This documentation assumes the following operations can be performed in $\mathcal{O}(1)$ time:

* Initializing an instance of `T`
* Adding and subtracting two instances of `T`
* Initializing an instance of `S`
* Computing $h(s_1, s_2)$ for two instances $s_1, s_2$ of `S`

<br>

## Constructor

```cpp
(1) kotone::dsu<T> d(int num_nodes)
(2) kotone::extended_dsu<S, op, T> d(int num_nodes)
(3) kotone::extended_dsu<S, op, T> d(int num_nodes, S init_x)
(4) kotone::extended_dsu<S, op, T> d(std::vector<S> vec)
```

Constructs a DSU for an undirected graph $G$ with no edges. Each node is initially in its own connected component.

* The number of nodes $N$ is specified by:
    * (1), (2), (3): `num_nodes`;
    * (4): the size of `vec`.
* The initial mapping $g$ for each connected component in `ed` is:
    * (2): a value-initialized instance of `S`;
    * (3): a copy of `init_x`;
    * (4): $g(\{v\})=$`vec[v]` for $v\in\{0, \dots, N-1\}$.
* `T` is `int` by default.

### Constraints
* `T` is default-constructible
* `T` supports addition (`+`) and subtraction (`-`) operators
* `S` is default-constructible
* `op` is associative

### Time complexity

* $\mathcal{O}(N)$

<br>

## Leader

```cpp
int d.leader(int v)
```

Returns the leader of the connected component containing $v$.

### Constraints

* $0\leq v\lt N$

### Time complexity

* Amortized $\mathcal{O}(\alpha(N))$

<br>

## Connectivity

```cpp
bool d.connected(int u, int v)
```

Returns whether $u$ and $v$ are connected. Equivalently, returns whether $u$ and $v$ belong to the same connected component.

### Constraints

* $0\leq u, v\lt N$

### Time complexity

* Amortized $\mathcal{O}(\alpha(N))$

<br>

## Merge

```cpp
(1) int d.merge(int u, int v)
(2) int d.merge(int u, int v, T pd)
```

Adds an edge between $u$ and $v$, then returns the leader of the merged connected component.

* If `d` is an `extended_dsu` and $u, v$ belong to distinct connected components, `d` also merges their images under $g$ via `op` and assigns the result to the new component.
* (1) `d` does not define the potential difference from $u$ to $v$, and subsequent calls to `potential_diff()` will trigger an assertion failure.
* (2) If $u, v$ are not formerly connected, defines `pd` as the potential difference from $u$ to $v$. If $u, v$ are already connected, discards `pd` to avoid redefinition.

### Constraints

* $0\leq u, v\lt N$

### Time complexity

* Amortized $\mathcal{O}(\alpha(N))$

<br>

## Potential difference

```cpp
T d.potential_diff(int u, int v)
```

Returns the potential difference from $u$ to $v$.

### Constraints

* $0\leq u, v\lt N$
* $u$ and $v$ are connected
* All previous `merge()` calls define potential differences

### Time complexity

* Amortized $\mathcal{O}(\alpha(N))$

<br>

## Connected component size

```cpp
int d.size(int v)
```

Returns the size of the connected component containing $v$.

### Constraints

* $0\leq v\lt N$

### Time complexity

* Amortized $\mathcal{O}(\alpha(N))$

<br>

## Connected components retrieval

```cpp
std::vector<std::vector<int>> d.components()
```

Returns a vector of connected components of $G$.

* The order of connected components is undefined.
* Each connected component is a vector of indices of member nodes in ascending order.

### Time complexity

* $\mathcal{O}(N)$

<br>

## Mapping retrieval (`extended_dsu` only)

```cpp
S d.get(int v)
```

Returns a copy of $g(C_v)$ where $C_v$ is the connected component containing $v$.

### Constraints

* $0\leq v\lt N$

### Time complexity

* Amortized $\mathcal{O}(\alpha(N))$

<br>

## Mapping assignment (`extended_dsu` only)

```cpp
int d.set(int v, S x)
```

Reassigns $g(C_v)$ as $x$ then returns the leader of $C_v$, where $C_v$ is the connected component containing $v$.

### Constraints

* $0\leq v\lt N$

### Time complexity

* Amortized $\mathcal{O}(\alpha(N))$

<br>

## Related problems (external links)

<details><summary>Click to unfold (spoilers)</summary>

* [ABC 235 E - MST + 1](https://atcoder.jp/contests/abc235/tasks/abc235_e)
* [ABC 328 F - Good Set Query](https://atcoder.jp/contests/abc328/tasks/abc328_f)

</details>

<br>
