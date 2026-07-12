#include <bits/stdc++.h>
using namespace std;

int main() {
	int C;
	cin >> C;
	while (C--) {
		int L, T;
		cin >> L >> T;

		vector<deque<bool>> bottom(L, deque<bool>(2*L-1, false));
		auto is_stable = [&bottom, L](int i, int j) {
			if (i == L-1) return true;
			if (!bottom[i+1][j]) return false;
			if (!bottom[i+1][j-1]) return false;
			if (!bottom[i+1][j+1]) return false;
			return true;
		};

		auto next_state = [&bottom, L](int i, int j) -> pair<int, int>{
			if (!bottom[i+1][j]) return {i+1, j};
			if (!bottom[i+1][j-1]) return {i+1, j-1};
			return {i+1, j+1};
		};

		for (int t = 0; t < T; ++t) {
			auto cur_state = make_pair(0, L-1);
			while (!is_stable(cur_state.first, cur_state.second)) {
				cur_state = next_state(cur_state.first, cur_state.second);
			}
			bottom[cur_state.first][cur_state.second] = true;
		}

		for (int i = 0; i < L; ++i) {
			for (int j = 0; j < L-i-1; ++j) {
				bottom[i].pop_back();
				bottom[i].pop_front();
			}
		}

		// Dibujar mitad superior
		for (int i = 0; i < L; ++i) {
			int fila = L - i - 1;
			// Espacios al principio
			for (int j = 0; j < i; ++j) cout << " ";
			cout << "\\";

			for (int j = bottom[fila].size() - 1; j >= 0; --j) {
				bool hueco = bottom[fila][j];
				cout << (hueco ? " " : "*");
			}
			cout << "/\n";
		}

		// Dibujar mitad inferior
		for (int i = 0; i < L; ++i) {
			// Espacios al principio
			for (int j = 0; j < L - 1 - i; ++j) cout << " ";
			cout << "/";

			for (auto grano : bottom[i]) {
				cout << (grano ? "*" : " ");
			}
			cout << "\\\n";
		}

		cout << "\n";
	}
}
