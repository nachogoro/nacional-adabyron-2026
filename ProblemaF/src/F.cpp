#include <bits/stdc++.h>
using namespace std;

// Caché: altura del árbol de raíz i
vector<int> height;

// Caché: ¿es robusto el árbol de raíz i?
vector<optional<bool>> robust;

// Caché: ¿un fallo en una sola hoja disminuiría la altura
// del árbol de raíz i?
vector<optional<bool>> can_shorten;

vector<vector<int>> adj;

int getHeight(int node) {
	if (height[node] != -1) return height[node];
	if (adj[node].empty()) {
		height[node] = 0;
		return 0;
	}

	int thisHeight = getHeight(adj[node][0]);
	if (adj[node].size() > 1) thisHeight = max(thisHeight, getHeight(adj[node][1]));
	height[node] = 1 + thisHeight;

	return height[node];
}

bool canShorten(int node) {
	if (can_shorten[node].has_value()) return can_shorten[node].value();

	// Si un nodo es hoja y raíz, su altura pasa de 1 a 0 si ella falla
	if (adj[node].empty()) {
		can_shorten[node] = true;
		return true;
	}

	if (adj[node].size() == 1) {
		// Si un nodo solo tiene un hijo, será acortable si su hijo es acortable
		can_shorten[node] = canShorten(adj[node][0]);
		return can_shorten[node].value();
	}

	// Si un nodo tiene dos hijos, será acortable si su altura proviene de uno
	// de los hijos y ese hijo es acortable
	int left = adj[node][0];
	int right = adj[node][1];
	int leftHeight = getHeight(left);
	int rightHeight = getHeight(right);

	if (leftHeight == rightHeight) {
		// Ambos subárboles tienen la misma altura. Incluso si uno pudiese
		// acortarse, nunca lo harán los dos a la vez
		can_shorten[node] = false;
		return false;
	}

	can_shorten[node] =(leftHeight > rightHeight && canShorten(left)) || (rightHeight > leftHeight && canShorten(right)) ;
	return can_shorten[node].value();
}

bool isRobust(int node) {
	if (robust[node].has_value()) return robust[node].value();

	for (int child : adj[node]) {
		if (!isRobust(child)) {
			robust[node] = false;
			return false;
		}
	}

	if (adj[node].empty()) {
		// Nodo hoja siempre es robusto
		robust[node] = true;
		return true;
	}

	auto [leftHeight, leftShortable] = make_tuple(getHeight(adj[node][0]), canShorten(adj[node][0]));

	auto [rightHeight, rightShortable] = adj[node].size() > 1
		? make_tuple(getHeight(adj[node][1]), canShorten(adj[node][1]))
		: make_tuple(0, false);

	auto [shortestHeight, shortestShortable] = leftHeight < rightHeight
		? make_tuple(leftHeight, leftShortable)
		: make_tuple(rightHeight, rightShortable);

	auto [tallestHeight, tallestShortable] = leftHeight < rightHeight
		? make_tuple(rightHeight, rightShortable)
		: make_tuple(leftHeight, leftShortable);

	if (shortestHeight == tallestHeight) {
		// Si los dos subárboles tienen la misma altura, incluso si uno pierde
		// altura van a seguir equilibrados
		robust[node] = true;
		return true;
	}

	if (shortestHeight < (tallestHeight - 1) || shortestShortable) {
		// Si los subárboles ya tienen una diferencia mayor que uno (no debería ocurrir)
		// o tienen una diferencia de uno pero el más corto puede acortarse si una
		// hoja fallase, la diferencia sería de dos y no serían robustos
		robust[node] = false;
		return false;
	}

	robust[node] = true;
	return true;
}

void solve() {
	int N;
	cin >> N;

	adj = vector<vector<int>>(N);
	height = vector<int>(N, -1);
	robust = vector<optional<bool>>(N, nullopt);
	can_shorten = vector<optional<bool>>(N, nullopt);

	for (int i = 0; i < N; ++i) {
		int k;
		cin >> k;
		for (int j = 0; j < k; ++j) {
			int child;
			cin >> child;
			adj[i].push_back(child-1);
		}
	}

	if(isRobust(0)) cout << "SI\n";
	else            cout << "NO\n";
}

int main() {
	int T;
	cin >> T;
	while (T--) {
		solve();
	}
}
