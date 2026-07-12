#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// La ruta es 0, a[0], a[1], ..., a[N-1], 0 (dos vertices extra en la posicion 0,
// al principio y al final). Su coste es la suma de distancias consecutivas.
//
// Al quitar una casa B, situada entre A y C en el recorrido, dejamos de recorrer
// las aristas A-B y B-C, pero anadimos la arista A-C. El resto no cambia. El
// ahorro de quitar B es por tanto
//     |A-B| + |B-C| - |A-C|   (>= 0 por la desigualdad triangular)
// Como hay que quitar exactamente una casa, elegimos la de mayor ahorro.

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(nullptr);

	int T;
	if (!(cin >> T))
		return 0;
	while (T--) {
		int N;
		cin >> N;
		vector<ll> a(N);
		for (auto& x : a)
			cin >> x;

		// Coste total de la ruta original (0 -> a[0] -> ... -> a[N-1] -> 0).
		ll total = 0, previa = 0;
		for (int i = 0; i < N; ++i) {
			total += llabs(a[i] - previa);
			previa = a[i];
		}
		total += llabs(previa); // regreso a 0

		// Mayor ahorro posible al quitar exactamente una casa.
		ll mejorAhorro = 0;
		for (int i = 0; i < N; ++i) {
			ll izq = (i == 0) ? 0 : a[i - 1];
			ll der = (i == N - 1) ? 0 : a[i + 1];
			ll ahorro = llabs(izq - a[i]) + llabs(a[i] - der) - llabs(izq - der);
			mejorAhorro = max(mejorAhorro, ahorro);
		}

		cout << total - mejorAhorro << '\n';
	}
	return 0;
}
