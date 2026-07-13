#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

int main(){
    int T; cin >> T;
    while (T--){
        int N, H; cin >> N >> H;
        
        ll A = 0;
        vector<ll> res(N+1,0);
        for (int i = 0; i < H; i++){
            int p, c; cin >> p >> c;
            if (p-c+1 > 0) res[p-c+1] += 1;
            else {
                A += 1;
                res[0] += c-p;
            }
            if (p+1 <= N) res[p+1] -= 2;
            if (p+c+1 <= N) res[p+c+1] += 1;
        }

        for (int i = 1; i <= N; i++){
            A += res[i];
            res[i] = res[i-1] + A;
        }

        for (int i = 1; i < N; i++) cout << res[i] << ' ';
        cout << res[N] << '\n';

    }
    return 0;
}