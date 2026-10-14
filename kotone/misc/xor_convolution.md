# XOR convolution

## Definition

A ring $R$ is a set equipped with addition and multiplication. In particular:

- $R$ is an abelian group under addition;
- $R$ is a monoid under multiplication;
- Multiplication is distributive with respect to addition.

Given sequences $A, B\in R^{2^n}$, the XOR convolution computes the sequence $C\in R^{2^n}$ satisfying

$$C_k=\sum_{i\oplus j=k}A_i B_j.$$

## Hadamard transform

The Hadamard matrix $H$ is defined as

$$
H_1=
\begin{pmatrix}
1
\end{pmatrix}
, H_{2^{n+1}}=
\begin{pmatrix}
H_{2^n} & H_{2^n}\\
H_{2^n} & -H_{2^n}
\end{pmatrix}
.
$$

Notably, $H_{2^n}H_{2^n}^\intercal=(H_{2^n})^2=2^nI_{2^n}$.

The Hadamard transform can be defined as multiplying a Hadamard matrix $H_{2^n}$ by a vector $a$ to obtain the transformed sequence $\hat{a}=H_{2^n}a$. The inverse Hadamard transform is $a=2^{-n}H_{2^n}\hat{a}$.

Let $H(S)=HS$ denote Hadamard transform on sequence $S$ and let $H^{-1}(S)=2^{-n}HS$ denote its inverse. The XOR convolution is given by

$$C=H^{-1}(H(A)\cdot H(B)).$$

```cpp
template <typename T> void hadamard(std::vector<T> &vec, int n) {
    for (int i = 0; i < n; i++) {
        for (int k = 0; k < 1 << n; k++) {
            if (~k >> i & 1) continue;
            T acc = vec[k];
            vec[k] = vec[k ^ 1 << i] - vec[k];
            vec[k ^ 1 << i] += acc;
        }
    }
}
```

## Related problems & articles

- [Library Checker - XOR Convolution](https://judge.yosupo.jp/problem/bitwise_xor_convolution)
- [ABC 212 H - Nim Counting](https://atcoder.jp/contests/abc212/tasks/abc212_h)
- [ABC 288 Ex - A Nameless Counting Problem](https://atcoder.jp/contests/abc288/tasks/abc288_h)
- [ABC 367 G - Sum of (XOR^K or 0)](https://atcoder.jp/contests/abc367/tasks/abc367_g)
- [ARC 222 E - XOR Matching](https://atcoder.jp/contests/arc222/tasks/arc222_e)
  - [Faster Hadamard transform](https://atcoder.jp/contests/arc222/editorial/21419)
- [AtCoder Algorithm Lectures - XOR 畳み込み（除算なし）](https://info.atcoder.jp/entry/algorithm_lectures/division_free_xor_convolution)
