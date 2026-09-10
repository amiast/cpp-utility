# Heavy-Light Decomposition

The `heavy_light_decomposition` header implements a technique that partitions undirected trees into directed subpaths. This transformation allows a class of tree problems to be solved using techniques related to one-dimensional sequences of data.

In heavy-light decomposition, edges are labeled as either "heavy" or "light", and each node has at most one heavy child.

In a tree with $N$ nodes, by designating the heavy child as the one with the largest subtree, each root-to-node path contains at most $\log_2 N$ light edges. This bound ensures that every path in the tree can be represented by a logarithmic number of subpaths.

<br>

## Construction

```cpp
kotone::heavy_light_decomposition_tree tree(std::vector<std::pair<int, int>> edges, int root = 0)
```

Constructs a rooted tree from the given undirected edges. The number of nodes is $N=M+1$ where $M$ is the size of `edges`.

### Constraints

- `edges` represents a valid undirected tree
- $0\leq u, v \lt N$ for all edges $(u, v)$

### Time complexity

- $\Theta(N)$

<br>

## Subtree size

```cpp
int tree.size(int v)
```

Returns the number of nodes in the subtree rooted at node $v$.

### Constraints

- $0\leq v \lt N$

### Time complexity

- $\Theta(1)$

<br>

## Parent

```cpp
int tree.parent(int v)
```

Returns the parent of node $v$. If $v$ is the root, returns $-1$ instead.

### Constraints

- $0\leq v \lt N$

### Time complexity

- $\Theta(1)$

<br>

## Head

```cpp
int tree.head(int v)
```

Returns the head of the subpath containing node $v$.

### Constraints

- $0\leq v \lt N$

### Time complexity

- $\Theta(1)$

<br>

## Order

```cpp
int tree.order(int v)
```

Returns the DFS pre-order position of node $v$.

### Constraints

- $0\leq v \lt N$

### Time complexity

- $\Theta(1)$

<br>

## Subpath decomposition

```cpp
std::vector<std::pair<int, int>> tree.to_subpaths(int u, int v)
```

Returns the $u$-$v$ path as a sequence of directed heavy subpaths $((s_0, t_0), \dots, (s_{k-1}, t_{k-1}))$.

- Each $(s_i, t_i)$ denotes the directed subpath from $s_i$ to $t_i$.
    - Nodes in the $s_i$-$t_i$ subpath are guaranteed to appear consecutively in the DFS pre-ordering.
- Guarantees $k\leq1+2\log_2 N$.

### Constraints

- $0\leq u, v \lt N$

### Time complexity

- $\mathcal{O}(\log N)$

<br>
