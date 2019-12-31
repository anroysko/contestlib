// Struct for priority queue operations on index set [1, n]
// push(i, v) overwrites value at position i if one already exists
struct Prique {
	const ll INF = 4 * (ll)1e18;
	vector<pair<ll, int>> data;

	Prique(int n) : data(2*n, {INF, -1}) {}
	void push(int i, ll v) {
		int j = i + data.size() / 2;
		for (data[j] = {v, i}; j > 1; j >>= 1) data[j>>1] = min(data[j], data[j^1]);
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
				que.push(t, off);
			}
		}
	}
	return {cost, prev};
}
