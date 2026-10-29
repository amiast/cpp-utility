# Throne

> Given positive integer $N$ and integers $0\leq A, D\lt N$, determine if there exist nonnegative integers $k$ for which $A+kD\equiv0\pmod{N}$. If they exist, find the minimum $k$.

### Implementation 1 (Python's built-in `pow`)
```python
def throne(N, A, D):
    g = gcd(N, D)
    if A % g != 0:
        return -1
    N, A, D = N // g, A // g, D //= g
    return -pow(D, -1, N) * A % N
```

### Implementation 2

```python
def extgcd(s, t):
    """ Returns `(x, y)` such that `s * x + t * y == gcd(s, t)`. """
    xs, ys = 1, 0
    xt, yt = 0, 1
    while t:
        u = s // t
        s -= t * u
        xs -= xt * u
        ys -= yt * u
        s, t = t, s
        xs, ys, xt, yt = xt, yt, xs, ys
    return xs, ys

def throne(N, A, D):
    g = gcd(N, D)
    if A % g != 0:
        return -1
    N, A, D = N // g, A // g, D //= g
    return -extgcd(N, D)[1] * A % N
```

### AtCoder Library

AtCoder Library's `inv_mod` also computes `pow(D, -1, N)` for coprime $N, D$.

### Related problems

- [ABC 186 E - Throne](https://atcoder.jp/contests/abc186/tasks/abc186_e)
- [ARC 221 A - Two Arithmetic Progressions](https://atcoder.jp/contests/arc221/tasks/arc221_a)
- [ABC 465 G - Sum of Mex of Mod of Linear](https://atcoder.jp/contests/abc465/tasks/abc465_g)

Other applications of the extended Euclidean algorithm:

- [ABC 315 G - Ai + Bj + Ck = X (1 <= i, j, k <= N)](https://atcoder.jp/contests/abc315/tasks/abc315_g)
- [ABC 340 F - S = 1](https://atcoder.jp/contests/abc340/tasks/abc340_f)
