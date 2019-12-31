// Struct for priority queue operations on
// index set [1, n] with really good constants
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
