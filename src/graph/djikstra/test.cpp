#include <iostream>
#include <vector>
#include <tuple>
using namespace std;
using ll = long long;

// Struct for priority queue operations on index set [1, n]
// push(i, v) overwrites value at position i if one already exists
// decKey is faster but requires that the new key is smaller than the old one
struct Prique {
	const ll INF = 4 * (ll)1e18;
	vector<pair<ll, int>> data;
	const int n;

	Prique(int siz) : n(siz), data(2*siz, {INF, -1}) { data[0] = {-INF, -1}; }
	void push(int i, ll v) {
		data[i+n] = {v, (v >= INF ? -1 : i)};
		for (i += n; i > 1; i >>= 1) data[i>>1] = min(data[i], data[i^1]);
	}
	void decKey(int i, ll v) {
		for (int j = i+n; data[j].first > v; j >>= 1) data[j] = {v, i};
	}
	pair<ll, int> pop() {
		auto res = data[1];
		push(res.second, INF);
		return res;
	}
};

// Find shortest paths from src to all other nodes.
// cost[i] is cost from src to node i
// prev[i] is previous node on cheapest path from src to i
// Complexity: O(m log n), Space: O(m)
pair<vector<ll>, vector<int>> djikstra(int src, const vector<vector<pair<int, ll>>>& g) {
	int n = g.size();
	vector<ll> cost(n, 4 * (ll)1e18);
	vector<int> prev(n, -1);
	Prique que(n);

	cost[src] = 0;
	for (int i = src; i != -1; i = que.pop().second) {
		for (auto pr : g[i]) {
			int t = pr.first;
			ll off = cost[i] + pr.second;
			if (off < cost[t]) {
				cost[t] = off;
				prev[t] = i;
				que.decKey(t, off);
			}
		}
	}
	return {cost, prev};
}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);
	
	int n, m, k;
	cin >> n >> m >> k;

	vector<vector<pair<int, ll>>> g(n);
	for (int j = 0; j < m; ++j) {
		int a, b, c;
		cin >> a >> b >> c;
		g[b-1].emplace_back(a-1, c);
	}
	vector<ll> res(n, 0);
	for (int j = 0; j < k; ++j) {
		int s;
		cin >> s;
		auto sub = djikstra(s-1, g);
		for (int i = 0; i < n; ++i) res[i] += sub.first[i];
	}
	for (auto v : res) cout << v << ' '; cout << '\n';
}
