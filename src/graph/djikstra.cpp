#include <iostream>
#include <vector>
#include <utility>
#include <assert.h>
using namespace std;
typedef long long ll;
typedef vector<vector<pair<int, ll>>> Graph;
const ll INF = 1e18;

// Struct for priority queue operations on
// index set [1, n] with really good constants
struct FastPrique {
	vector<pair<ll, int>> data;
	FastPrique(int n, ll ini = INF) : data(2*n, {ini, -1}) {
		data[0] = {-INF, -1};
	}
	void decKey(int i, ll v) {
		int j = i + data.size() / 2;
		while(data[j].first > v) {
			data[j] = {v, i};
			j >>= 1;
		}
	}
	pair<ll, int> pop() {
		auto res = data[1];
		int j = res.second + data.size() / 2;
		data[j] = {INF, -1};
		for (; j > 1; j >>= 1) {
			data[j >> 1] = min(data[j], data[j^1]);
		}
		return res;
	}
	bool empty() {
		return data[1].first == INF;
	}
};

// Find shortest paths from src to all other nodes.
// cost[i] is cost from src to node i
// prev[i] is previous node on cheapest path from src to i
// Complexity: O(m log n)
pair<vector<ll>, vector<int>> djikstra(int src, const Graph& g) {
	int n = g.size();
	vector<ll> cost(n, INF);
	vector<int> prev(n, -1);

	FastPrique que(n);
	que.decKey(src, 0);
	cost[src] = 0;
	
	while(! que.empty()) {
		int i = que.pop().second;
		for (auto ed : g[i]) {
			int t = ed.first;
			ll offer = cost[i] + ed.second;
			if (offer < cost[t]) {
				que.decKey(t, offer);
				cost[t] = offer;
				prev[t] = i;
			}
		}
	}
	return {cost, prev};
}

// Example usage
int main() {
	int n, m, s;
	cin >> n >> m >> s;
	--s;

	Graph g(n);
	for (int i = 0; i < m; ++i) {
		int a, b; ll c;
		cin >> a >> b >> c;
		--a; --b;
		g[a].push_back({b, c});
	}

	auto res = djikstra(s, g);
	for (auto v : res.first) cout << v << ' '; cout << '\n';
	for (auto v : res.second) cout << v << ' '; cout << '\n';
}
