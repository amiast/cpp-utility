# from kotone.misc.string_concat import *
# https://github.com/amiast/cpp-utility

def z_algorithm(s):
    n = len(s)
    if n == 0:
        return []
    z = [0] * n
    j = 0
    for i in range(1, n):
        if j + z[j] <= i:
            z[i] = 0
        else:
            z[i] = min(j + z[j] - i, z[i - j])
        while i + z[i] < n and s[z[i]] == s[i + z[i]]:
            z[i] += 1
        if j + z[j] < i + z[i]:
            j = i
    z[0] = n
    return z

class ConcatStr(str):
    def __new__(cls, val):
        obj = super().__new__(cls, val)
        obj.z = z_algorithm(val)
        return obj
    def cmp(self, other):
        n, m = len(self), len(other)
        if n < m:
            return -other.cmp(self)
        for k in range(m):
            if self[k] != other[k]:
                return -1 if self[k] < other[k] else 1
        if n == m:
            return 0
        if self.z[m] < n - m:
            return -1 if self[m + self.z[m]] < self[self.z[m]] else 1
        for k in range(m):
            if other[k] != self[n - m + k]:
                return -1 if other[k] < self[n - m + k] else 1
        return 0
    def __eq__(self, other): return self.cmp(other) == 0
    def __ne__(self, other): return self.cmp(other) != 0
    def __lt__(self, other): return self.cmp(other) == -1
    def __le__(self, other): return self.cmp(other) != 1
    def __gt__(self, other): return self.cmp(other) == 1
    def __ge__(self, other): return self.cmp(other) != -1
