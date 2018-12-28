#include <iostream>
#include <vector>
using namespace std;
typedef long long ll;

// Dinic's algorithm for max flow
// Source must be node 0, sink must be node n-1
// All edges are directed
// O(V^2 E) in general
// O(min(V^{2/3}, sqrt{E}) E) in graphs with unit capacities
// O(sqrt{V} E) in unit networks (e.g. bipartite matching)
struct Dinic {
	private:
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

	// Update dist (building level graph)
	void calcDists() {
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
	}
	// DFS-push flow in the level graph, however at most mx.
	ll dfsFlow(int i, ll mx) {
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

	public:
	void init(int n, int src, int sink) {
		flow_src = src;
		flow_tar = sink;
		conns.resize(n);
		dist.resize(n);
	}

	// Adds the given edge, and returns its index
	// Edges are directed. To add an undirected edge, call as
	// addEdge(src, tar, 2*cap, cap) or add two edges.
	int addEdge(int src, int tar, ll cap, ll flow = 0) {
		int i = edges.size();
		edges.emplace_back(src, tar, cap, flow);
		conns[src].push_back(i);
		conns[tar].push_back(i);
		return i;
	}

	ll pushFlow() {
		ll res = 0;
		while(true) {
			calcDists();
			if (dist[flow_src] == dist.size()) return res;
			res += dfsFlow(flow_src, 8*(ll)1e18);
		}
		return res;
	}

	ll getEdgeFlow(int edge_id) const {
		return edges[edge_id].flow;
	}
};

// Example usage
int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);

	int n, m;
	cin >> n >> m;

	Dinic dinic;
	dinic.init(n, 0, n-1);

	for (int i = 0; i < m; ++i) {
		int a, b;
		ll c;
		cin >> a >> b >> c;
		--a; --b;
		dinic.addEdge(a, b, c);
	}
	
	ll flow = dinic.pushFlow();
	cout << flow << '\n';
}
