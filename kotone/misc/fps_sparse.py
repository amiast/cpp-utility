# from kotone.misc.fps_sparse import *
# https://github.com/amiast/cpp-utility

#
# Sparse FPS represented as list of tuples `[(i_1, f_1), (i_2, f_2), ...]`
# where `i_1 < i_2 < ...` and `1 <= f_1, f_2, ... < mod`.
# Returns full FPS represented as list of ints `[g_1, g_2, ...]`
#

def inverse(f: list[tuple[int, int]], n: int, mod: int) -> list[int]:
    """ Given sparse FPS `f` with coefficients modulo `mod`, returns `f^{-1} mod x^n`.

        Requires `len(f) >= 1 and f[0][0] == 0 and n >= 0 and mod > 0`.
        Requires `f` to be a valid representation of sparse FPS.
    """
    assert(len(f) >= 1 and f[0][0] == 0 and n >= 0 and mod > 0)
    assert(all(f[i][0] < f[i + 1][0] for i in range(len(f) - 1)))
    assert(all(1 <= f[i][1] < mod for i in range(len(f))))
    g = [pow(f[0][1], -1, mod)] * n
    for i in range(1, n):
        g[i] = g[i] * -sum(c * g[i - j] for j, c in f[1:] if i - j >= 0) % mod
    return g

def logarithm(f: list[tuple[int, int]], n: int, mod: int) -> list[int]:
    """ Given sparse FPS `f` with coefficients modulo `mod`, returns `log(f) mod x^n`.

        Requires `len(f) >= 1 and f[0] == (0, 1) and n >= 0 and mod > 0`.
        Requires `f` to be a valid representation of sparse FPS.
    """
    assert(len(f) >= 1 and f[0] == (0, 1) and n >= 0 and mod > 0)
    assert(all(f[i][0] < f[i + 1][0] for i in range(len(f) - 1)))
    assert(all(1 <= f[i][1] < mod for i in range(len(f))))
    inv = [1] * n
    for i in range(2, n):
        inv[i] = mod // i * -inv[mod % i] % mod
    g = [0] * n
    for j, c in f[1:]:
        g[j] = c
    for i in range(1, n):
        g[i] = (g[i] - inv[i] * sum(c * (i - j) % mod * g[i - j] % mod for j, c in f[1:] if i - j >= 0)) % mod
    return g

def exponential(f: list[tuple[int, int]], n: int, mod: int) -> list[int]:
    """ Given sparse FPS `f` with coefficients modulo `mod`, returns `exp(f) mod x^n`.

        Requires `n >= 0 and mod > 0`.
        Requires `f` to be a valid representation of sparse FPS.
    """
    assert(n >= 0 and mod > 0)
    assert(all(f[i][0] < f[i + 1][0] for i in range(len(f) - 1)))
    assert(all(1 <= f[i][1] < mod for i in range(len(f))))
    g = [1] * n
    for i in range(2, n):
        g[i] = mod // i * -g[mod % i] % mod
    for i in range(1, n):
        g[i] = g[i] * sum(c * j % mod * g[i - j] % mod for j, c in f if i - j >= 0) % mod
    return g

def power(f: list[tuple[int, int]], k: int, n: int, mod: int) -> list[int]:
    """ Given sparse FPS `f` with coefficients modulo `mod`, returns `f^k mod x^n`.

        Assumes `0^0 == 1`.

        Requires `n >= 0 and mod > 0`.
        Requires `f` to be a valid representation of sparse FPS.
    """
    assert(n >= 0 and mod > 0)
    assert(all(f[i][0] < f[i + 1][0] for i in range(len(f) - 1)))
    assert(all(1 <= f[i][1] < mod for i in range(len(f))))
    if n == 0:
        return []
    if not f:
        if k == 0:
            return [1] + [0] * (n - 1)
        return [0] * n

    d = f[0][0]
    if k > 0 and d >= (n + k - 1) // k:
        print(*[0] * n)
        exit()

    a = f[0][1]
    pow_a = pow(a, k, mod)
    inv_a = pow(a, -1, mod)
    h = [(f[i][0] - d, f[i][1] * inv_a % mod) for i in range(len(f))]

    shift = d * k
    n -= shift
    inv = [1] * n
    for i in range(2, n):
        inv[i] = mod // i * -inv[mod % i] % mod
    k %= mod
    g = [0] * n
    for j, c in h:
        if j >= n:
            continue
        if j == 0:
            g[j] = 1
        else:
            g[j] = c * k % mod
    for i in range(1, n):
        g[i] = (g[i] + inv[i] * sum((j * (k + 1) - i) * c % mod * g[i - j] for j, c in h[1:] if i - j > 0)) % mod
    result = [0] * (n + shift)
    for i in range(n):
        result[i + shift] = g[i] * pow_a % mod
    return result
