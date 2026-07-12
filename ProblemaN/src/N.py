import functools
from math import ceil

N = int(input())
while (N != 0):
    p = list(map(float,input().split()))
    p.sort(reverse=True)

    @functools.cache
    def dp(c,b):
        if (b > c or b < 0): return 0
        if (c == 0 and b == 0): return 1
        pc = p[c-1]
        return dp(c-1,b-1)*pc + dp(c-1,b)*(1-pc)
    
    res = 0
    for c in range(1,N+1):
        P = 0
        for b in range(ceil((N+c)/3),c+1): P += dp(c,b)
        res = max(res,P)

    print(res)
    N = int(input())
