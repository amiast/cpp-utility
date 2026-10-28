# Many kinds of convolution

Convolution refers to the operation that computes a sequence $C$ from sequences $A$ and $B$ such that

$$C_k=\bigoplus_{i\circ j=k}(A_i\otimes B_j)$$

where $\circ$, $\oplus$ and $\otimes$ are abstract binary operations. This article assumes these operations can be performed in $\mathcal{O}(1)$ time.

Whereas naive convolution on sequences of length at most $n$ takes $\Theta(n^2)$ time, certain transforms can be used to achieve $o(n^2)$ time complexity. Below is a table of things that can be convolved in $o(n^2)$ time, adapted from [noshi91のメモ - 私用メモ: 畳み込めるものまとめ](https://noshi91.hatenablog.com/entry/2020/10/27/175112).

|Name|$\circ$|$\oplus$|$\otimes$|Time complexity|Transform|
|-|:-:|:-:|:-:|-|-|
|Conv.|$+$|$+$|$\times$|$\mathcal{O}(n\log n)$|FFT, NTT|
|AND conv.|$\cap$|$+$|$\times$|$\mathcal{O}(n\log n)$|Zeta and Möbius trans.|
|OR conv.|$\cup$|$+$|$\times$|$\mathcal{O}(n\log n)$|Zeta and Möbius trans.|
|XOR conv.|$\bigtriangleup$|$+$|$\times$|$\mathcal{O}(n\log n)$|Hadamard trans.|
|Subset conv.|$\sqcup$|$+$|$\times$|$\mathcal{O}(n\log^2n)$|Zeta and Möbius trans., lift and unlift|
|GCD conv.|$\gcd$|$+$|$\times$|$\mathcal{O}(n\log\log n)$|Zeta and Möbius trans. See [`moebius_inversion.md`](moebius_inversion.md)|
|LCM conv.|$\text{lcm}$|$+$|$\times$|$\mathcal{O}(n\log\log n)$|Zeta and Möbius trans. See [`moebius_inversion.md`](moebius_inversion.md)|
|Dirichlet conv.|$\times$|$+$|$\times$|$\mathcal{O}(n\log n)$|-|
|-|$\times\pmod p$|$+$|$\times$|$\mathcal{O}(n\log n)$|NTT and primitive root. See [AGC047C](https://atcoder.jp/contests/agc047/tasks/agc047_c)|
|Convex min-plus conv.|$+$|$\min$|$+$|$\mathcal{O}(n)$|-|

## Dirichlet convolution

### Definition

Given sequences $A, B$, the Dirichlet convolution computes a sequence $C=A\ast B$ such that

$$C_k=\sum_{ij=k}A_iB_j.$$

The first $n$ terms of $C$ can be computed in $\mathcal{O}(n\log n)$ time:

```python
C = [0] * (n + 1)
for i in range(1, n + 1):
    for j in range(1, n // i + 1):
        C[i * j] += A[i] * B[j]
```

Another way of expressing Dirichlet convolution is to write $D_C=D_AD_B$, where $D_a$ is the Dirichlet series of sequence $a$:

$$D_a(s)=\sum_{k=1}^\infty\frac{a_k}{k^s}.$$

### Multiplicative functions

A function $f$ is said to be multiplicative if $f(1)=1$ and $f(pq)=f(p)f(q)$ for coprime $p,q$. For such a function, $f(r)$ can be reconstructed from $f(p^k)$ for all prime powers $p^k$ dividing $r$. The Dirichlet series of $f$ is therefore

$$D_f(s)=\prod_p\sum_{k=0}^\infty \frac{f(p^k)}{p^{ks}}.$$

This expansion is also known as an **Euler product**.

Dirichlet convolution of $A,B$ can be performed in $\mathcal{O}(n\log\log n)$ time whenever $A$ is multiplicative. For example, the LCM convolution utilizes this property as seen in [`moebius_inversion.md`](moebius_inversion.md). The following implementation updates $B$ in place.

```python
for p in primes:
    for j in range(n // p, 0, -1):
        k, q, m = p * j, p, j
        while True:
            B[k] += A[q] * B[m]
            if m % p != 0:
                break
            q, m = q * p, m // p
```

If both $A, B$ are multiplicative, an implementation in $\mathcal{O}(n)$ time is also possible.

### Prefix sum

Let $\mathcal{A}_k=\sum_{i=1}^kA_i$ denote the prefix sum of $A$, and define $\mathcal{B},\mathcal{C}$ similarly for $B,C$, respectively.

To compute

$$\mathcal{C}_n=\sum_{ij\leq k}A_iB_j,$$

the sum on the right-hand side can be split into cases where $i\leq m$ and $i\gt m$ where $m=\lfloor\sqrt n\rfloor$. If $i\leq m$, then its contribution is $A_i\mathcal{B}_{\lfloor n/i\rfloor}$. If $i\gt m$, then $j\leq m$ and $m\lt i\leq n/j$, so its contribution is $(\mathcal{A}_{\lfloor n/j\rfloor}-\mathcal{A}_m)B_j$.

Thus, if $A,B,\mathcal{A},\mathcal{B}$ are precomputed, then $\mathcal{C}_n$ can be computed in $\mathcal{O}(\sqrt n)$ time.

The number of elements needed to compute $\mathcal{C}_n$ is sparse. In particular, it only takes $\mathcal{O}(n^{3/4})$ time to compute all of $C_i$ and $\mathcal{C}_{\lfloor n/i\rfloor}$ for $i\leq\sqrt n$ from $A_i, B_i, \mathcal{A}_{\lfloor n/i\rfloor}, \mathcal{B}_{\lfloor n/i\rfloor}$.

With better pivots, $C$ and $\mathcal{C}$ can be computed in $\mathcal{O}(n^{2/3}\log^{1/3}n)$ time. It can further improve to $\mathcal{O}(n^{2/3}\log^{1/3}\log n)$ if one of $A$ and $B$ is multiplicative, and to $\mathcal{O}(n^{2/3})$ if both $A$ and $B$ are multiplicative.

### Related problems & articles

- [maspyのHP - Dirichlet 積と、数論関数の累積和](https://maspypy.com/dirichlet-%E7%A9%8D%E3%81%A8%E3%80%81%E6%95%B0%E8%AB%96%E9%96%A2%E6%95%B0%E3%81%AE%E7%B4%AF%E7%A9%8D%E5%92%8C)
- [yukicoder - トーティエント関数$\varphi(i)$の和$\sum_{i=1}^N\varphi(i)$を$O(N^{2/3}(\log\log N)^{1/3})$で求める](https://yukicoder.me/wiki/sum_totient)
- [Library Checker - Sum of Totient Function](https://judge.yosupo.jp/problem/sum_of_totient_function)
- [ABC 162 E - Sum of gcd of Tuples (Hard)](https://atcoder.jp/contests/abc162/tasks/abc162_e)
- [ABC 239 Ex - Dice Product 2](https://atcoder.jp/contests/abc239/tasks/abc239_h)
- [ABC 428 G - Necklace](https://atcoder.jp/contests/abc428/tasks/abc428_g)

## Min-plus convolution

Given $A=(A_0, \dots, A_n)$ and $B=(B_0, \dots, B_m)$, compute

$$C_k=\min_{i+j=k}(A_i+B_j)=\min_j(A_{k-j}+B_j)$$

for all $k\in\lbrace0, \dots, n+m\rbrace$.

When $A$ and $B$ are convex, min-plus convolution can be performed efficiently:

- Min-plus where $A, B$ are convex
    - [AtCoder Algorithm Lectures - 凸数列の min-plus 畳み込み](https://info.atcoder.jp/entry/algorithm_lectures/min_plus_convolution)
    - [Library Checker - Min Plus Convolution (Convex and Convex)](https://judge.yosupo.jp/problem/min_plus_convolution_convex_convex)
- Min-plus where $A$ is convex
    - [Library Checker - Min Plus Convolution (Convex and Arbitrary)](https://judge.yosupo.jp/problem/min_plus_convolution_convex_arbitrary)
    - [ABC 348 G - Max (Sum - Max)](https://atcoder.jp/contests/abc348/tasks/abc348_g) - max-plus where $A$ is concave
- Min-plus where $A$ is concave (!)
    - [Library Checker - Min Plus Convolution (Concave and Arbitrary)](https://judge.yosupo.jp/problem/min_plus_convolution_concave_arbitrary)
