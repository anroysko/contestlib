#include <iostream>
#include <vector>
#include <utility>
using namespace std;
typedef long long ll;

// Dinic's algorithm for max flow
// Source must be node 0, sink must be node n-1
// All edges are directed
// O(V^2 E) in general
// O(min(V^{2/3}, sqrt{E}) E) in graphs with unit capacities
// O(sqrt{V} E) in unit networks (e.g. bipartite matching)
struct Dinic {
	struct Edge {
		const int src;
		const int tar;
		const ll cap;
		ll flow;

		Edge(int a, int b, ll c, ll f = 0) : src(a), tar(b), cap(c) {
			flow = f;
		}
	};
	static int getOth(int i, const Edge& ed) {
		return (i == ed.src ? ed.tar : ed.src);
	}
	static ll getCap(int i, const Edge& ed) {
		return (i == ed.src ? ed.cap - ed.flow : ed.flow);
	}
	static void push(int i, Edge& ed, ll am) {
		ed.flow += (i == ed.src ? am : -am);
	}

	vector<Edge> edges;
	vector<vector<int>> conns; // List of adjanced edges
	vector<int> dist; // Distance to sink
	int flow_src; // source
	int flow_tar; // sink

	bool calcDists() {
		for (int i = 0; i < dist.size(); ++i) dist[i] = dist.size();
		dist[flow_tar] = 0;
		vector<int> que = {flow_tar};
		for (int j = 0; j < que.size(); ++j) {
			int i = que[j];
			for (int ei : conns[i]) {
				int t = getOth(i, edges[ei]);
				ll cap = getCap(t, edges[ei]);
				if (cap > 0 && dist[t] > dist[i]+1) {
					dist[t] = dist[i]+1;
					que.push_back(t);
				}
			}
		}
		return dist[flow_src] < dist.size(); // Return true if sink is reachable from source
	}
	ll dfsFlow(int i, ll mx = 8*(ll)1e18) {
		if (i == flow_tar || mx == 0) return mx;
		ll res = 0; // Amount of flow we end up pushing
		for (int ei : conns[i]) {
			int t = getOth(i, edges[ei]);
			if (dist[t] != dist[i]-1) continue; // edge not in level graph
			ll cap = getCap(i, edges[ei]);
			ll pc = dfsFlow(t, min(mx - res, cap));
			push(i, edges[ei], pc);
			res += pc;
		}
		return res;
	}

	void init(int n, int src, int sink) {
		flow_src = src;
		flow_tar = sink;
		conns.resize(n);
		dist.resize(n);
	}
	int addEdge(int src, int tar, ll cap, ll flow = 0) {
		int i = edges.size();
		edges.emplace_back(src, tar, cap, flow);
		conns[src].push_back(i);
		conns[tar].push_back(i);
		return i; // Returns index of added edge
	}
	ll pushFlow() {
		ll res = 0;
		while(calcDists()) res += dfsFlow(flow_src);
		return res;
	}
	const Edge& getEdge(int edge_id) const {
		return edges[edge_id];
	}
};

struct BipMatcher {
	Dinic solver;
	vector<bool> side;

	void init(const vector<bool>& sid) {
		side = sid;
		int n = side.size();
		solver.init(n+2, 0, n+1);
		for (int i = 1; i <= n; ++i) {
			if (side[i-1]) solver.addEdge(i, n+1, 1);
			else solver.addEdge(0, i, 1);
		}
	}

	void addEdge(int a, int b) {
		if (side[a]) swap(a, b);
		solver.addEdge(a+1, b+1, 1);
	}

	vector<pair<int, int>> getMaxMatching() {
		solver.pushFlow();
		vector<pair<int, int>> res;
		for (int i = side.size(); i < solver.edges.size(); ++i) {
			auto ed = solver.getEdge(i);
			if (ed.flow > 0) {
				res.push_back({ed.src-1, ed.tar-1});
			}
		}
		return res;
	}

	vector<int> getMinVertexCover() {
		solver.pushFlow();
		solver.calcDists();
		// See wikipedia page for könig's theorem for explanation of this algorithm
		// Here it's inverted with respect to L and R
		vector<int> k;
		for (int i = 0; i < side.size(); ++i) {
			bool z = (solver.dist[i+1] != solver.dist.size());
			if (side[i] != z) {
				k.push_back(i);
			}
		}
		return k;
	}
};

// Example usage
int main() {
	int n, m;
	cin >> n >> m;
	vector<bool> side(n);
	for (int i = 0; i < n; ++i) {
		char c;
		cin >> c;
		side[i] = (c == 'L'); // L or R
	}
	
	BipMatcher matcher;
	matcher.init(side);
	for (int i = 0; i < m; ++i) {
		int a, b;
		cin >> a >> b;
		--a; --b;
		matcher.addEdge(a, b);
	}

	auto res = matcher.getMaxMatching();
	cout << "Number of pairs: " << res.size() << ", pairs:\n";
	for (auto pr : res) cout << pr.first << ' ' << pr.second << '\n';

	auto cov = matcher.getMinVertexCover();
	cout << "Number of nodes: " << cov.size() << ", nodes:\n";
	for (auto i : cov) cout << i << ' '; cout << '\n';
}
