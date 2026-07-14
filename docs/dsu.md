# DSU

The `dsu` is a data structure that maintains disjoint subsets in a partition of a set. This data structure is also known as union-find (UF). This documentation represents subsets of a partition as connected components of an undirected graph. The implementation is based on that of [the Atcoder Library](https://github.com/atcoder/ac-library) with additional maintenance of **potential difference** and **connected component mapping**.

Given an undirected graph $G=(V, E)$ with $N$ nodes labeled $0, \dots, N-1$, a DSU can add edges between nodes and check connectivity in $\mathcal{O}(\alpha(N))$ amortized time per operation. Here, $\alpha$ denotes the inverse Ackermann function. For each connected component of $G$, the DSU assigns a member node as its leader. When the DSU merges two connected components, it reassigns a member node as the new leader.

The potential difference between two connected nodes in $G$ is a function $f:C\rightarrow T$ with the following properties:

- $C\subseteq V^2$ is the set of pairs of connected nodes in $G$
- $T$ is an additive group
- $f(u, v)=-f(v, u)$
- For all simple paths $(v_1, \dots, v_k)$ in $G$, $f(v_1, v_k)=\sum_{i=1}^{k-1}f(v_i, v_{i+1})$


With user-provided methods, the DSU also maps connected components with commutative monoids. See [Construction](#constructor) for more information.

This documentation assumes the following operations can be performed in $\mathcal{O}(1)$ time:

- `pd_type`: initialization, addition, and subtraction
- `on_init`

<br>

## Constructor

```cpp
(1) kotone::dsu d()
(2) kotone::dsu d(int num_nodes)

(3) kotone::dsu<pd_type> d()
(4) kotone::dsu<pd_type> d(int num_nodes)

(5) kotone::dsu<pd_type, on_init, on_merge> d()
(6) kotone::dsu<pd_type, on_init, on_merge> d(int num_nodes)
```

Constructs a DSU for an undirected graph $G$ with no edges. Each node is initially in its own connected component.

- The number of nodes $N$ is:
    - (1), (3), (5): $0$; or
    - (2), (4), (6): `num_nodes`.
- The numeric type of potential difference is:
    - (1), (2): `int`; or
    - (3-6): `pd_type`.
- (5), (6): Uses the following functions:
    - Calls `void on_init(int node)` after adding a new node to $G$.
    - Calls `void on_merge(int new_leader, int old_leader)` after merging two distinct connected components.

### Constraints

- `pd_type` default-initializes to $0$.
- `pd_type` supports `operator+` and `operator-`.

### Time complexity

- $\mathcal{O}(N)$

<br>

## Graph size

```cpp
int d.num_nodes()
```

Returns $N$.

### Time complexity

- $\mathcal{O}(1)$

<br>

## Vertex insertion

```cpp
int d.add_vertex()
```

Adds a new vertex to $G$ and returns its index.

- Calls `on_init` if provided.

### Time complexity

- Amortized $\mathcal{O}(1)$

<br>

## Leader

```cpp
int d.leader(int v)
```

Returns the leader of the connected component containing $v$.

### Constraints

- $0\leq v\lt N$

### Time complexity

- Amortized $\mathcal{O}(\alpha(N))$

<br>

## Connectivity

```cpp
bool d.connected(int u, int v)
```

Returns whether $u$ and $v$ are connected.

### Constraints

- $0\leq u, v\lt N$

### Time complexity

- Amortized $\mathcal{O}(\alpha(N))$

<br>

## Merge

```cpp
(1) int d.merge(int u, int v)
(2) int d.merge(int u, int v, pd_type pd)
```

Adds an edge between $u$ and $v$, then returns the leader of the merged connected component.

- If $u, v$ are connected, this method does not modify $G$.
- (1) If $u, v$ are disconnected, sets $0$ as the potential difference from $u$ to $v$.
- (2) If $u, v$ are disconnected, sets `pd` as the potential difference from $u$ to $v$.
- If provided, calls `void on_merge(int new_leader, int old_leader)` after the merge is complete.
    - Does not call `on_merge` if $u, v$ are already connected.

### Constraints

- $0\leq u, v\lt N$

### Time complexity

- Amortized $\mathcal{O}(\alpha(N)+T(N))$, where $T(N)$ is the amortized computational cost of `on_merge`.

<br>

## Potential difference

```cpp
pd_type d.potential_diff(int u, int v)
```

Returns the potential difference $f(u, v)$.

### Constraints

- $0\leq u, v\lt N$
- $u$ and $v$ are connected

### Time complexity

- Amortized $\mathcal{O}(\alpha(N))$

<br>

## Connected component size

```cpp
int d.size(int v)
```

Returns the size of the connected component containing $v$.

### Constraints

- $0\leq v\lt N$

### Time complexity

- Amortized $\mathcal{O}(\alpha(N))$

<br>

## Connected components retrieval

```cpp
std::vector<std::vector<int>> d.components()
```

Returns a vector of connected components of $G$.

- The order of connected components is undefined.
- Each connected component is a vector of indices of member nodes in ascending order.

### Time complexity

- $\mathcal{O}(N)$

<br>

## Related problems (external links)

<details><summary>Click to unfold (spoilers)</summary>

- [ABC 280 F - Pay or Receive](https://atcoder.jp/contests/abc280/tasks/abc280_f)
- [ABC 328 F - Good Set Query](https://atcoder.jp/contests/abc328/tasks/abc328_f)
- [ABC 383 E - Sum of Max Matching](https://atcoder.jp/contests/abc383/tasks/abc383_e)
- [ABC 451 F - Make Bipartite 3](https://atcoder.jp/contests/abc451/tasks/abc451_f)

</details>

<br>
