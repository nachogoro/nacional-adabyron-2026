#include <bits/stdc++.h>
using namespace std;
using ll = long long;

const ll MOD = 1e9 + 7;

ll potencia(ll base, ll exp) {
	ll r = 1 % MOD;
	base %= MOD;
	while (exp > 0) {
		if (exp & 1) r = r * base % MOD;
		base = base * base % MOD;
		exp >>= 1;
	}
	return r;
}

// Cuenta las iluminaciones validas de un anillo de N bombillas en el que no
// puede haber mas de K encendidas consecutivas (contando tambien el cierre del
// anillo). Es decir, cadenas binarias circulares de longitud N sin ninguna
// racha de mas de K unos.
//
// Recorremos el anillo llevando como estado la longitud de la racha de
// encendidas en curso (0..K): apagar vuelve al estado 0, encender pasa de i a
// i+1, y desde el estado K no se puede encender. El numero de anillos validos
// es la traza de M^N, con M la matriz de transferencia (K+1)x(K+1) de esas
// transiciones: cuenta los recorridos cerrados de longitud N. Definimos
//   P_n = traza(M^n) = anillos validos de longitud n con al menos un apagado
// (el "todo encendido" nunca es un recorrido cerrado, porque la racha no puede
// envolver el anillo entero sin pasar de K).
//
// Por Cayley-Hamilton, M cumple su polinomio caracteristico, que aqui es
// x^{K+1} = x^K + ... + x + 1, y tomando trazas se obtiene
//   P_n = P_{n-1} + P_{n-2} + ... + P_{n-K-1}   para n >= K+1.
// Las semillas son P_0 = K+1 (la traza de la identidad) y, para 1 <= n <= K,
// como con longitud <= K cualquier cadena con algun apagado ya es valida,
//   P_n = 2^n - 1   (todas menos el "todo encendido").
//
// La respuesta es P_N: con K < N el "todo encendido" no es valido, asi que no
// falta ningun caso. El caso K = N (cabe encender el anillo entero) se resuelve
// aparte: son las 2^N cadenas posibles.
ll iluminaciones(ll N, ll K) {
	if (K >= N) return potencia(2, N);

	vector<ll> P(N + 1);
	P[0] = (K + 1) % MOD;

	ll pot = 1; // 2^n mod MOD
	for (ll n = 1; n <= K; ++n) {
		pot = pot * 2 % MOD;
		P[n] = (pot - 1 + MOD) % MOD; // 2^n - 1
	}

	ll ventana = 0; // suma de los ultimos K+1 valores calculados
	for (ll i = 0; i <= K; ++i)
		ventana = (ventana + P[i]) % MOD;
	for (ll n = K + 1; n <= N; ++n) {
		P[n] = ventana;
		ventana = (ventana + P[n] - P[n - K - 1] + MOD) % MOD;
	}

	return P[N];
}

int main() {
	cin.tie(nullptr);
	ios_base::sync_with_stdio(false);

	int T;
	cin >> T;
	while (T--) {
		ll N, K;
		cin >> N >> K;
		cout << iluminaciones(N, K) << '\n';
	}
	return 0;
}
