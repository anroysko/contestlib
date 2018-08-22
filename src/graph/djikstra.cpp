#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
using namespace std;
typedef long long ll;
const ll INF = 1e18;

// For better cache efficiency (two-dimensional arrays suck at this)
// Can be removed, is easy to replace.
template<class T>
struct TwoDimArray {
	vector<T> data;
	vector<int> starts;

	void init(const vector<vector<T>>& vec) {
		int n = vec.size();
		int m = 0;
		for (int i = 0; i < n; ++i) m += vec[i].size();
		
		starts.resize(n+1);
		data.resize(m);
		int j = 0;
		for (int i = 0; i < n; ++i) {
			starts[i] = j;
			for (int ind = 0; ind < vec[i].size(); ++ind) {
				data[j+ind] = vec[i][ind];
			}
			j += vec[i].size();
		}
		starts[n] = m;
	}
	T& operator()(int x, int y) {
		return data[starts[x] + y];
	}
	const T& operator()(int x, int y) const {
		return data[starts[x] + y];
	}
	int size(int x) const {
		return starts[x+1] - starts[x];
	}
};

// A min-segment tree that works as a priority queue.
// only keys [0, n) can have their values set. Push on a set index overrides it.
// decKey can be used to create a new element.
// Only supports values <= INF.
// All operations are log(n), with very fast constants.
struct SegPriQue {
	vector<ll> vals;
	
	void init(int n) {
		vals.resize(2*n, INF + 1);
	}

	bool empty() {
		return (vals[1] == INF + 1);
	}

	void push(int i, ll v) {
		int n = vals.size() / 2;
		i += n;
		vals[i] = v;
		while(i > 1) {
			vals[i >> 1] = min(vals[i], vals[i ^ 1]);
			i >>= 1;
		}
	}

	void decKey(int i, ll v) {
		int n = vals.size() / 2;
		i += n;
		while((i > 0) && (vals[i] > v)) {
			vals[i] = v;
			i >>= 1;
		}
	}

	pair<int, ll> top() {
		int n = vals.size() / 2;
		int i = 1;
		ll v = vals[1];
		while(i < n) {
			int le = i<<1;
			if (vals[le] == v) i = le;
			else i = le+1;
		}
		return {i-n, v};
	}
	
	pair<int, ll> pop() {
		auto res = top();
		push(res.first, INF + 1);
		return res;
	}
};

// Find shortest path from src to tar.
// To find shortest paths to all nodes, pick tar = -1.
// If tar != -1, cost/prev is not necessarily defined for nodes not on shortest path to tar.
// Cost of cheapest path to node i is costs[i]. Previous node on path to node i is prev[i]. prev[src] == -1.
// Complexity: O(m log n)
// Preconditions: cost and prev have size n.
void djikstra(int src, int tar, vector<ll>& cost, vector<int>& prev, const TwoDimArray<pair<int, int>>& edges) {
	int n = cost.size();
	for (int i = 0; i < n; ++i) cost[i] = INF;

	SegPriQue que;
	que.init(n);
	que.push(src, 0);
	cost[src] = 0;
	prev[src] = -1;
	
	while(! que.empty()) {
		int i = que.pop().first;
		if (i == tar) return;

		for (int ti = 0; ti < edges.size(i); ++ti) {
			auto tec = edges(i, ti);
			int t = tec.first;
			ll offer = cost[i] + tec.second;
			
			if (offer < cost[t]) {
				que.decKey(t, offer);
				cost[t] = offer;
				prev[t] = i;
			}
		}
	}
}

// Prints for every node, what is the shortest distance to that node from node 0,
// and what is the previous node on the optimal path.
int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);

	int n, m, s, t;
	cin >> n >> m >> s >> t;
	--s; --t;

	vector<vector<pair<int, int>>> tmp_edges(n);
	for (int i = 0; i < m; ++i) {
		int a, b; ll c;
		cin >> a >> b >> c;
		--a; --b;
		tmp_edges[a].push_back({b, c});
	}

	TwoDimArray<pair<int, int>> edges;
	edges.init(tmp_edges);
	
	vector<ll> cost(n);
	vector<int> prev(n);
	
	djikstra(s, t, cost, prev, edges);
	for (auto v : cost) cout << v << ' '; cout << '\n';
	for (auto v : prev) cout << v << ' '; cout << '\n';
}
