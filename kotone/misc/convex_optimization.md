# Convex optimization

This article introduces topics related to convex optimization for discrete problems.

# Submodular function

A submodular function is a function $f:\mathcal{P}(\Omega)\rightarrow\mathbb{R}$, where $\Omega$ is a finite set and $\mathcal{P}(\Omega)$ is its powerset, that satisfies any of the following equivalent conditions:

1. For $X\subseteq Y\subseteq\Omega$ and $x\in \Omega\setminus Y$,
$$f(X\cup\lbrace x\rbrace)-f(X)\geq f(Y\cup\lbrace x\rbrace)-f(Y).$$
2. For $S, T\subseteq\Omega$,
$$f(S)+f(T)\geq f(S\cup T)+f(S\cap T).$$
3. For $X\subseteq\Omega$ and distinct $x_1,x_2\in\Omega\setminus X$,
$$f(X\cup\lbrace x_1\rbrace)+f(X\cup\lbrace x_2\rbrace)\geq f(X\cup\lbrace x_1,x_2\rbrace)+f(X).$$

The property also describes **diminishing returns** between increasing input and decreasing marginal output.

Some notable submodular functions include:
- monotone submodular functions: $f(X)\leq f(Y)$ for $X\subseteq Y\subseteq\Omega$.
- symmetric submodular functions: $f(S)=f(\Omega\setminus S)$ for $S\subseteq\Omega$.

## Properties

- Non-negative linear combinations of submodular functions are also submodular.
- If $f$ is submodular, then $g(S)=f(\Omega\setminus S)$ is also submodular.
- If $f$ is monotone submodular and $h$ is non-decreasing and concave, then $g(S)=h(f(S))$ is submodular.

# Monge property

For integer intervals $[\ell, r)$, let $c(\ell, r)$ be a cost function for which $-c(\ell, r)$ is submodular. Define a directed graph of $N+1$ vertices, labeled $0, \dots, N$, where a directed edge from $u$ to $v$ has cost $c(u, v)$ for all $u\lt v$. We aim to find the minimum cost of a $k$-edge path from $0$ to $N$.

Since the negation of $c$ is submodular, for all $0\leq x\lt y\lt z\lt w\leq N$,

$$c(x, w)+c(y, z)\geq c(x, z)+c(y, w).$$

Since the above property also appears in Monge matrices, we extend the concept and refer to this property as the Monge property. In particular, the lower-triangular matrix $M$ defined by $M_{ij}=c(j, i)$ is a Monge matrix.

Let $d(k)$ be the minimum cost of a $k$-edge path from vertex $0$ to vertex $N$. It can be shown that $d$ is convex. That is, for $2\leq n\leq N$,

$$d(n)-d(n-1)\geq d(n-1)-d(n-2).$$

Since $d$ is convex, $d(k)$ can be computed via the Aliens method.

# Aliens method

> There are $N$ distinguishable balls. Your goal is to choose $K$ balls under certain constraints to maximize a concave function $f$. Find $f(K)$.

The above problem may be solved using DP in $O(T(N)K)$ time where $T(N)$ is the computational cost of an iteration. A different approach is to consider the following modification:

> There are $N$ balls. You may choose **any number of balls**. When you choose $k$ balls, your score is $f(k)-pk$ for a specified constant $p$. Find the **maximum** $k$ that maximizes $f(k)-pk$.

Since $f$ is concave, there exists $p$ for which $k=K$ maximizes $f(k)-pk$. Geometrically, $p$ is the slope of a supporting line intersecting $f$ at $K$. Furthermore, the maximum $k$ that maximizes $f(k)-pk$ is non-increasing as a function of $p$.

If the codomain of $f$ is the set of integers, then all marginal values $f(k)-f(k-1)$ are integers, hence an integer $p$ can be chosen for which $k=K$ maximizes $f(k)-pk$. In this case, $p$ can be computed via binary search in $\mathcal{O}(T(N)\log(\max p-\min p))$ time to recover $f(K)=(f(K)-pK)+pK$. Here, $\max p$ and $\min p$ are the maximum and minimum values of $f(k)-f(k-1)$, respectively.

```python
def maximize(K, min_p, max_p):
    def solve(p): ... # Returns (f(k)-p*k, k) where k is the maximum value maximizing f(k)-p*k.
        ...
    low = min_p
    high = max_p + 1
    while low + 1 < high:
        mid = (low + high) // 2
        if solve(mid)[1] >= K:
            low = mid
        else:
            high = mid
    return solve(low)[0] + low * K
```

This method is known as the Aliens trick, named after IOI 2016 - Aliens, among a handful of other names.

Note that the value of $k$ maximizing $f(k)-pk$ may not be unique. The above formulation chooses the maximum such $k$ for binary search. It is also possible to use other consistent tiebreakers.

Similarly, to minimize a convex function $f$, it suffices to find $p$ for which $k=K$ minimizes $f(k)-pk$. The maximum $k$ that minimizes $f(k)-pk$ is non-decreasing as a function of $p$.

```python
def minimize(K, min_p, max_p):
    def solve(p): ... # Returns (f(k)-pk, k) where k is the maximum value minimizing f(k)-pk.
    low = min_p - 1
    high = max_p
    while low + 1 < high:
        mid = (low + high) // 2
        if solve(mid)[1] >= K:
            high = mid
        else:
            low = mid
    return solve(high)[0] + high * K
```

Sometimes, it may be easy to compute the maximum (or minimum) $f(k)-pk$ but difficult to recover a particular value of $k$. Let $g(p)=\max\lbrace f(k)-pk\rbrace+pK$. Whereas $g(p)\geq f(K)$ for all $p$, the slope $p^\prime$ of a supporting line at $K$ satisfies $g(p^\prime)=f(K)$, so $f(K)=\min g(p)$.

Since $g(p)$ is the maximum of affine functions of $p$, it is convex. Thus, $\min g(p)$ can be searched in logarithmic time via ternary search or golden-section search.

```python
def maximize(K, min_p, max_p):
    def solve(p): ... # Returns max(f(k)-pk)+pK.
    low = min_p
    high = max_p
    while low + 2 < high:
        mid_l = (low * 2 + high) // 3
        mid_r = (low + high * 2) // 3
        if solve(mid_l) < solve(mid_r):
            high = mid_r
        else:
            low = mid_l
    return min(solve(low), solve(low + 1), solve(high))
```

## Related articles and problems
- https://noshi91.github.io/algorithm-encyclopedia/d-edge-shortest-path-monge
- https://info.atcoder.jp/entry/algorithm_lectures/alien_trick
- [ABC 218 H - Red and Blue Lamps](https://atcoder.jp/contests/abc218/tasks/abc218_h) - maximization
- [ABC 305 Ex - Shojin](https://atcoder.jp/contests/abc305/tasks/abc305_h) - minimization
- [ARC 168 E - Subsegments with Large Sums](https://atcoder.jp/contests/arc168/tasks/arc168_e) - minimization with special binary search conditions
- [ABC 355 G - Baseball](https://atcoder.jp/contests/abc355/tasks/abc355_g) - Monge minimization with ternary search via Larmore–Schieber DP (online algorithm for totally monotone, lower-triangular matrix); see also SMAWK algorithm
- [ABC 462 F - More ABC](https://atcoder.jp/contests/abc462/tasks/abc462_f) - minimization
- [灘校文化祭コンテスト 2022 Day2 K - Exhibition 3](https://atcoder.jp/contests/nadafes2022_day2/tasks/nadafes2022_day2_k) - maximization
- [TUPC 2022 K - Lebesgue Integral](https://atcoder.jp/contests/tupc2022/tasks/tupc2022_k) - Monge maximization with ternary search
- [JOI 2022/2023 Spring Training G - Chorus](https://atcoder.jp/contests/joisp2023/tasks/joisp2023_g) - Monge minimization; similar to ABC 355 G
- [JOI 2025/2026 Final Stage H - Scarecrows 2](https://atcoder.jp/contests/joi2026final/tasks/joi2026final_h) - minimization; reduction from special case of weighted bipartite matching

# Matroid

A **matroid** $M$ is a pair $(E, \mathcal{I})$ where the **ground set** $E$ is finite and its **independent sets** $\mathcal{I}$ is a subset of the power set of $E$.

A matroid $M=(E, \mathcal{I})$ satisfies the following properties:

- The empty set is independent.
- Every subset of an independent set is also independent.
- If $X, Y\in\mathcal{I}$ and $|X|\gt|Y|$, then there exists $x\in X\setminus Y$ such that $Y\cup\lbrace x\rbrace\in\mathcal{I}$.

A **basis** of a matroid is a maximal independent set. By the above properties, all bases of a matroid have the same size. The **rank** of a matroid is the size of a basis.

## Examples

The **free matroid** of a finite set $E$ is $(E, \mathcal{P}(E))$.

The **uniform matroid** of rank $r$ is a matroid where the independent sets are all subsets whose size is at most $r$.

A **partition matroid** is a direct sum of uniform matroids.

Given a set $E$ in a vector space $V$, if $\mathcal{I}$ is the linearly independent subsets of $E$, then $(E, \mathcal{I})$ is a **vector matroid**. The **column matroid** of a matrix is defined similarly such that its independent sets are the linearly indpendent columns of the matrix. A basis of the column matroid corresponds to a basis of the matrix.

Given a finite graph $G=(V, E)$, the **cycle matroid** or **graphic matroid** $M(G)=(E, \mathcal{I})$ is defined such that $I$ is the family of all edge sets that form a forest. Thus, a basis of a cycle matroid is a maximal forest containing a spanning tree of each connected component of $G$.

## Matroid intersection

The matroid intersection problem asks for the largest common independent set of two matroids over the same ground set. That is, to find $X\in\mathcal{I}_1\cap\mathcal{I}_2$ that maximizes $|X|$ given matroids $(E, \mathcal{I}_1)$ and $(E, \mathcal{I}_2)$.

A known algorithm solves the matroid intersection problem in $\mathcal{O}(|E|^2r)$ time where $r$ is the size of the maximum independent set.

The matroid intersection problem is a generalization of many graph problems, including maximum matchings in bipartite graphs.

Consider a bipartite graph $G=(U\cup V, E)$. Since every vertex $u\in U$ is matched with at most one edge, the constraint can be expressed as a partition matroid. Similarly, the constraint on $V$ can also be expressed as a matroid. Thus, the maximum matching is their maximum common independent set.

- [Implementation by hitonanode](https://hitonanode.github.io/cplib-cpp/combinatorial_opt/matroid_intersection.hpp.html)

## Linear matroid intersection

Let $A_1, A_2$ be two matrices with the same number of columns. The linear matroid intersection problem concerns their column matroids $M_1, M_2$.

Let $E$ be the column set and define indeterminates $x_1, \dots, x_{|E|}$. Let $D$ be the $|E|\times|E|$ polynomial matrix whose diagonal entries are $x_e$. The size of a maximum independent set is equivalent to $\text{rank}(A_1DA_2^T)$.

A randomized algorithm can compute the rank in $\mathcal{O}(nm^2)$ time where $n,m$ are the numbers of rows of $A_1$ and $A_2$.

Consider again the maximum matchings on a bipartite graph $G=(U\cup V, E)$. The Edmonds matrix $A$ of $G$ is defined by $A_{uv}=x_{uv}$ if $(u,v)\in E$, otherwise $A_{uv}=0$. We show that the size of a maximum matching is equal to $\text{rank }A$.

The previous section defines partition matroids with respect to $U$ and $V$, and these matroids can in turn be described using column matroids. Let $e_i$ be a column whose $i$-th entry is $1$ and other entries are $0$. Let $A_1$ be a $|E|$-column matrix whose $i$-th column is $e_u$ where $(u, v)$ is the $i$-th edge, and similarly define $A_2$ in terms of $e_v$. Their corresponding polynomial matrix $A_1DA_2^T$ is equal to

$$\sum_{(u, v)\in E}e_ue_v^Tx_{uv},$$

which is identical to the Edmonds matrix.

## Greedy algorithm for matroids

Consider the problem of maximizing the weight of an independent set where each element $x\in E$ has a nonnegative weight $w(x)$. The corresponding greedy algorithm is as follows:

1. Let $S$ be an empty set.
2. For each $x\in E$ in descending order of $w(x)$, if $S\cup\lbrace x\rbrace\in\mathcal{I}$, add $x$ to $S$.
3. Output $S$.

## Aliens method for matroids

> There is a connected, weighted simple graph $G=(V, E)$. Furthermore, each edge $e\in E$ is colored either black or white. Find the minimum weight of a spanning tree that contains exactly $K$ black edges.

The above problem can be solved by iterating Kruskal's algorithm with respect to some constant $p$. In this case, the weight of a black edge $e$ is $w(e)-p$, whereas the weight of a white edge $e^\prime$ remains $w(e^\prime)$. The above problem can hence be solved in $\mathcal{O}(|E|\log|E|)$ time.

This approach can be generalized other matroids when minimizing the cost of a basis containing $K$ elements of a certain category.

## Related problems

- [ABC 137 D - Summer Vacation](https://atcoder.jp/contests/abc137/tasks/abc137_d)
- [ABC 363 G - Dynamic Scheduling](https://atcoder.jp/contests/abc363/tasks/abc363_g)
- [ABC 399 G - Colorful Spanning Tree](https://atcoder.jp/contests/abc399/tasks/abc399_g)
- [JSC 2019 Qualification E - Card Collector](https://atcoder.jp/contests/jsc2019-qual/tasks/jsc2019_qual_e)
