#include <bits/stdc++.h>
using namespace std;

int main(){
	int N;

	while ((cin >> N) && N){
		vector<double> p(N);
		for (auto &pi : p) cin >> pi;
		sort(p.rbegin(),p.rend());

		vector<double> prev(N+1, 0);
		// Probabilida del 100% de no acertar ninguna pregunta si no
		// respondemos ninguna pregunta
		prev[0] = 1.0;
		double best_chance = 0.0;

		for (int c = 1; c <= N; c++) {
			double pc = p[c-1];

			vector<double> next(N+1, 0);

			// Probabilidad de aprobar si se responden exactamente estas c preguntas
			double aprobar_con_c = 0.0;

			for (int b = 0; b <= c; b++) {
				double fallar  = prev[b]*(1. - pc);                // fallar la c-ésima
				double acertar = (b >= 1)   ? prev[b-1]*pc : 0.0;  // acertar la c-ésima
				next[b] = fallar + acertar;

				// ¿Está aprobado el examen respondiendo b bien de c respondidas?
				bool aprobado = (b - 0.5*(c - b)) >= (N / 2.0);

				// Aprobar respondiendo c es lograr CUALQUIER número de aciertos
				// que supere el umbral, así que sumamos todos esos escenarios.
				if (aprobado) {
					aprobar_con_c += next[b];
				}
			}

			best_chance = max(best_chance, aprobar_con_c);
			swap(prev, next);
		}

		cout << fixed << setprecision(6) << best_chance << '\n';
	}
	return 0;
}
