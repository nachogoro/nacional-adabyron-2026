#include <bits/stdc++.h>
using namespace std;

#define MAX 110

int main(){
	int N; cin >> N;

	while (N){
		vector<double> p(N);
		for (auto &pi : p) cin >> pi;
		sort(p.rbegin(),p.rend());

		double dp[MAX][MAX] = {};
		dp[0][0] = 1;

		for (int c = 1; c <= N; c++) {
			for (int b = 0; b <= c; b++) {
				double pc = p[c-1];
				dp[c][b] = dp[c-1][b]*(1.-pc) + (b ? dp[c-1][b-1]*pc : 0.);
			}
		}

		double res = 0.;
		for (int c = 1; c <= N; c++) {
			double P = 0.;
			for (int b = (N+c)/3 + ((N+c)%3 ? 1 : 0); b <= c; b++) {
				P += dp[c][b];
			}
			res = max(P,res);
		}

		cout << res << '\n';
		cin >> N;
	}
	return 0;
}
