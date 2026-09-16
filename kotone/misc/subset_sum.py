# from kotone.misc.subset_sum import *
# https://github.com/amiast/cpp-utility

def subset_sum(nums: list[int], target: int) -> list[bool] | None:
    """ Determines if there is a subset that sums to `target`.
        If one exists, returns a `list[bool]` for whether each element is used.
        If none exists, returns `None`.

        Requires `all(n >= 0 for n in nums)`.

        Time complexity: O(len(nums) * max(nums))

        Reference: https://atcoder.jp/contests/abc221/editorial/2741
        See also: https://codeforces.com/blog/entry/98663
    """
    if not nums:
        return [] if target == 0 else None
    assert(all(n >= 0 for n in nums))
    S = sum(nums)
    if not (0 <= target <= S):
        return None
    N = len(nums)
    M = max(nums)

    acc = 0
    l = 0
    while l < N and acc + nums[l] <= target:
        acc += nums[l]
        l += 1

    # offset == target - (M - 1)
    dp = [-1] * (M * 2)
    parent = [[-1] * (M * 2) for _ in range(N)]
    dp[acc - target + M - 1] = l
    for i in range(l, N):
        ndp = dp[:]
        prev = parent[i]
        for j in range(M - 1, -1, -1):
            if ndp[j + nums[i]] < ndp[j]:
                prev[j + nums[i]] = -2
                ndp[j + nums[i]] = ndp[j]
        for j in range(M * 2 - 1, M - 1, -1):
            for k in range(ndp[j] - 1, max(dp[j], 0) - 1, -1):
                if ndp[j - nums[k]] < k:
                    prev[j - nums[k]] = k
                    ndp[j - nums[k]] = k
        dp = ndp

    if dp[M - 1] == -1:
        return None
    result = [False] * N
    i = N - 1
    j = M - 1
    while i >= l:
        p = parent[i][j]
        if p == -2:
            result[i] = not result[i]
            j -= nums[i]
            i -= 1
        elif p == -1:
            i -= 1
        else:
            result[p] = not result[p]
            j += nums[p]
    while i >= 0:
        result[i] = not result[i]
        i -= 1
    return result
