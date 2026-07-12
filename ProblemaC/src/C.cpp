#include <bits/stdc++.h>
using namespace std;

// Modelamos un grafo cuyos vertices son las ZONAS CRUJIENTES (a lo sumo 200).
// Dos zonas estan conectadas (coste 1) si comparten una region de suelo firme,
// es decir, si se puede caminar de una a otra por suelo firme sin pisar ninguna
// otra zona crujiente. El coste de un camino es el numero de zonas que atraviesa.
//
// Para una consulta origen->destino (ambos en suelo firme):
//   - Si estan en la misma region firme: 0 zonas.
//   - Si no, la respuesta es min sobre (zo bordeando el origen, zd bordeando el
//     destino) de 1 + dist(zo, zd). Equivale a anadir un vertice de salida con
//     aristas de coste 1 a sus zonas y uno de llegada con aristas de coste 0, y
//     lanzar un Dijkstra / BFS. Como hay <= 200 zonas, precalculamos todas las
//     distancias entre pares con un BFS desde cada zona.

const int INF = INT_MAX / 2;
const int DR[4] = {-1, 1, 0, 0};
const int DC[4] = {0, 0, -1, 1};

int F, C;                 // filas y columnas del plano
vector<string> plano;     // el mapa (estado global del caso)

inline int celda(int r, int c) { return r * C + c; }
inline bool dentro(int r, int c) { return r >= 0 && r < F && c >= 0 && c < C; }

// Etiqueta por inundacion las celdas iguales a `tipo`. Rellena `comp` (id de
// componente por celda, -1 si no aplica) y devuelve cuantas componentes hay.
int etiquetarComponentes(char tipo, vector<int>& comp)
{
	comp.assign(F * C, -1);
	int n = 0;
	vector<int> pila;
	for (int r = 0; r < F; ++r)
		for (int c = 0; c < C; ++c) {
			if (plano[r][c] != tipo || comp[celda(r, c)] != -1)
				continue;
			comp[celda(r, c)] = n;
			pila.push_back(celda(r, c));
			while (!pila.empty()) {
				int v = pila.back(); pila.pop_back();
				int vr = v / C, vc = v % C;
				for (int d = 0; d < 4; ++d) {
					int nr = vr + DR[d], nc = vc + DC[d];
					if (dentro(nr, nc) && plano[nr][nc] == tipo && comp[celda(nr, nc)] == -1) {
						comp[celda(nr, nc)] = n;
						pila.push_back(celda(nr, nc));
					}
				}
			}
			++n;
		}
	return n;
}

// Para cada region firme, el conjunto (ordenado y sin repetir) de zonas crujientes
// que la bordean.
vector<vector<int>> zonasQueBordeanCadaRegion(const vector<int>& region,
                                              const vector<int>& zona, int numRegiones)
{
	vector<vector<int>> zonas(numRegiones);
	for (int r = 0; r < F; ++r)
		for (int c = 0; c < C; ++c) {
			if (plano[r][c] != '.')
				continue;
			int reg = region[celda(r, c)];
			for (int d = 0; d < 4; ++d) {
				int nr = r + DR[d], nc = c + DC[d];
				if (dentro(nr, nc) && plano[nr][nc] == '*')
					zonas[reg].push_back(zona[celda(nr, nc)]);
			}
		}
	for (auto& v : zonas) {
		sort(v.begin(), v.end());
		v.erase(unique(v.begin(), v.end()), v.end());
	}
	return zonas;
}

// Lista de adyacencia del grafo de zonas: dos zonas que comparten una region son
// adyacentes (coste 1).
vector<vector<int>> construirGrafoDeZonas(const vector<vector<int>>& zonasPorRegion,
                                          int numZonas)
{
	vector<vector<int>> ady(numZonas);
	vector<vector<char>> conectadas(numZonas, vector<char>(numZonas, 0));
	for (const auto& zonas : zonasPorRegion)
		for (size_t i = 0; i < zonas.size(); ++i)
			for (size_t j = i + 1; j < zonas.size(); ++j) {
				int a = zonas[i], b = zonas[j];
				if (!conectadas[a][b]) {
					conectadas[a][b] = conectadas[b][a] = 1;
					ady[a].push_back(b);
					ady[b].push_back(a);
				}
			}
	return ady;
}

// Distancia (numero de zonas) entre cada par de zonas, con un BFS desde cada una.
vector<vector<int>> distanciasEntreZonas(const vector<vector<int>>& ady, int numZonas)
{
	vector<vector<int>> dist(numZonas, vector<int>(numZonas, INF));
	for (int s = 0; s < numZonas; ++s) {
		dist[s][s] = 0;
		vector<int> cola = {s};
		for (size_t h = 0; h < cola.size(); ++h) {
			int u = cola[h];
			for (int w : ady[u])
				if (dist[s][w] == INF) {
					dist[s][w] = dist[s][u] + 1;
					cola.push_back(w);
				}
		}
	}
	return dist;
}

// Zonas minimas a atravesar entre dos regiones firmes (o -1 si no hay camino).
int zonasAAtravesar(int regOrigen, int regDestino,
                    const vector<vector<int>>& zonasPorRegion,
                    const vector<vector<int>>& dist)
{
	if (regOrigen == regDestino)
		return 0; // misma region: no hay que pisar nada crujiente
	int mejor = INF;
	for (int zo : zonasPorRegion[regOrigen])
		for (int zd : zonasPorRegion[regDestino])
			mejor = min(mejor, 1 + dist[zo][zd]);
	return mejor >= INF ? -1 : mejor;
}

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(nullptr);

	int T;
	if (!(cin >> T))
		return 0;
	while (T--) {
		cin >> C >> F;
		plano.resize(F);
		for (int r = 0; r < F; ++r)
			cin >> plano[r];

		vector<int> region, zona;
		int numRegiones = etiquetarComponentes('.', region);
		int numZonas = etiquetarComponentes('*', zona);

		auto zonasPorRegion = zonasQueBordeanCadaRegion(region, zona, numRegiones);
		auto ady = construirGrafoDeZonas(zonasPorRegion, numZonas);
		auto dist = distanciasEntreZonas(ady, numZonas);

		int Q;
		cin >> Q;
		while (Q--) {
			int x1, y1, x2, y2;
			cin >> x1 >> y1 >> x2 >> y2;
			int regOrigen = region[celda(y1 - 1, x1 - 1)];
			int regDestino = region[celda(y2 - 1, x2 - 1)];
			cout << zonasAAtravesar(regOrigen, regDestino, zonasPorRegion, dist) << '\n';
		}
		cout << "---\n";
	}
	return 0;
}
