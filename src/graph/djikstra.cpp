#include <ext/pb_ds/priority_queue.hpp>
#include <iostream>
#include <vector>
#include <utility>
using namespace std;
typedef long long ll;
const ll INF = 1e18;

// thin heap (better fib heap)
// O(1) insert, join, reduce_key, O(log(n)) pop.
// Speeds up djikstra from O(m log(m)) -> O(m + n log(n))
// This should be used with large cases where m > n.
template<class T>
using fib_heap = __gnu_pbds::priority_queue<T, greater<T>, __gnu_pbds::thin_heap_tag>;

template<class T>
struct TwoDimArray {
	vector<T> data;
	vector<int> starts;

	void init(int n, const vector<pair<int, T>>& vec) {
		starts.resize(n+1);
		data.resize(vec.size());
		starts[n] = vec.size();

		int pi = 0;
		starts[0] = 0;
		for (int i = 0; i < vec.size(); ++i) {
			while(pi <= vec[i].first) {
				starts[pi] = i;
				++pi;
			}
			data[i] = vec[i].second;
		}
		while(pi <= n) {
			starts[pi] = vec.size();
			++pi;
		}
	}
	const T& operator()(int x, int y) const {
		return data[starts[x] + y];
	}
	int size(int x) const {
		return starts[x+1] - starts[x];
	}
};

// Find shortest path from src to tar.
// To find shortest paths to all nodes, pick tar = -1.
// Cost of cheapest path to node i is costs[i].
// Previous node on path to the node is prev[i]. prev[src] == -1.
// Complexity: O(m + n log n)
// Preconditions: cost and prev have size n.
void djikstra(int src, int tar, const TwoDimArray<pair<int, ll>>& edges, vector<ll>& cost, vector<int>& prev) {
	int n = cost.size();
	for (int i = 0; i < n; ++i) cost[i] = INF;

	vector<fib_heap<pair<ll, int>>::point_iterator> its (n);
	fib_heap<pair<ll, int>> heap;
	heap.push({0, src});
	cost[src] = 0;
	prev[src] = -1;
	
	while(! heap.empty()) {
		int j = heap.top().second;
		if (j == tar) return;
		heap.pop();

		for (int ti = 0; ti < edges.size(j); ++ti) {
			auto tec = edges(j, ti);
			int t = tec.first;
			ll offer = cost[j] + tec.second;
			
			if (offer < cost[t]) {
				if (cost[t] == INF) its[t] = heap.push({offer, t});
				else heap.modify(its[t], {offer, t});
				cost[t] = offer;
				prev[t] = j;
			}
		}
	}
}

// Prints for every node, what is the shortest distance to that node from node 0,
// and what is the previous node on the optimal path.
int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);

	int n, m;
	cin >> n >> m;

	vector<pair<int, pair<int, ll>>> tmp_edges(m);
	for (int i = 0; i < m; ++i) {
		cin >> tmp_edges[i].first >> tmp_edges[i].second.first >> tmp_edges[i].second.second;
		--tmp_edges[i].first;
		--tmp_edges[i].second.first;
	}
	sort(tmp_edges.begin(), tmp_edges.end());

	TwoDimArray<pair<int, ll>> edges;
	edges.init(n, tmp_edges);
	tmp_edges.clear();
	tmp_edges.shrink_to_fit();

	vector<ll> cost(n);
	vector<int> prev(n);
	djikstra(0, -1, edges, cost, prev);
	for (int i = 0; i < n; ++i) cout << cost[i] << ' '; cout << '\n';
	for (int i = 0; i < n; ++i) cout << prev[i] << ' '; cout << '\n';
}
