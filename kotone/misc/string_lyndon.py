# from kotone.misc.string_lyndon import *
# https://github.com/amiast/cpp-utility
# See also https://atcoder.jp/contests/abc258/editorial/4256
# See also https://judge.yosupo.jp/problem/lyndon_factorization

def min_rotation(S):
    N = len(S)
    i, j, k = 0, 1, 0
    while i < N and j < N and k < N:
        if S[(i + k) % N] == S[(j + k) % N]:
            k += 1
        elif S[(i + k) % N] > S[(j + k) % N]:
            i = max(i + k + 1, j + 1)
            k = 0
        else:
            j = max(j + k + 1, i + 1)
            k = 0
    return min(i, j)

def lyndon_factorization(S):
    N = len(S)
    result = []
    l = 0
    while l < N:
        m = l
        r = l + 1
        while r < N and S[m] <= S[r]:
            if S[m] == S[r]:
                m += 1
                r += 1
            else:
                m = l
                r += 1
        w = r - m
        rep = (r - l) // w
        for k in range(rep):
            result.append(l + k * w)
        for i in range(w):
            if l + i + w * rep == N or S[l + i] > S[l + i + w * rep]:
                break
            if S[l + i] < S[l + i + w * rep]:
                return result + [N]
        l += w * rep
    return result + [N]
