#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
using namespace std;
using ll = long long;
const ll INF = 1e18;

struct Edge {
	int s, t, w;
	int oth(int x) {
		return x == s ? t : s;
	}
};
using Graph = vector<vector<Edge>>;

// Find shortest paths from src to all other nodes.
// cost[i] is cost from src to node i
// prev[i] is previous node on cheapest path from src to i
// Complexity: O(m log n), Space: O(m log m)
pair<vector<ll>, vector<int>> djikstra(int src, const Graph& g) {
	int n = g.size();
	vector<ll> cost(n, INF);
	vector<int> prev(n, -1);

	priority_queue<pair<ll, int>> que;
	que.push({0, src});
	cost[src] = 0;
	
	while(! que.empty()) {
		int w = -que.top().first;
		int i = que.top().second;
		que.pop();
		if (w != cost[i]) continue;

		for (auto ed : g[i]) {
			int t = ed.oth(i);
			ll offer = w + ed.w;
			if (offer < cost[t]) {
				que.push({-offer, t});
				cost[t] = offer;
				prev[t] = i;
			}
		}
	}
	return {cost, prev};
}

// Example usage
int main() {
	int n, m, src;
	cin >> n >> m >> src;
	--src;

	Graph g(n);
	for (int i = 0; i < m; ++i) {
		int a, b, c;
		cin >> a >> b >> c;
		--a; --b;
		g[a].push_back({a, b, c});
		// g[b].push_back({a, b, c}); // uncomment if undirected
	}

	vector<ll> cost;
	vector<int> prev;
	tie(cost, prev) = djikstra(src, g);

	for (auto v : cost) cout << v << ' '; cout << '\n';
	for (auto v : prev) cout << v << ' '; cout << '\n';
}
