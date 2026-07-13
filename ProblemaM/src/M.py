T = int(input())
for _ in range(T):
    N, H = list(map(int,input().split()))
    res = [0 for _ in range(N+1)]
    A = 0
    for _ in range(H):
        p, c = list(map(int,input().split()))
        if p-c+1 > 0: res[p-c+1] += 1
        else:
            A += 1
            res[0] += c-p
        if p+1 <= N: res[p+1] -= 2
        if p+c+1 <= N: res[p+c+1] += 1
    
    for i in range(1,N+1):
        A += res[i]
        res[i] = res[i-1] + A
    
    print(" ".join(list(map(str,res))[1:]))
