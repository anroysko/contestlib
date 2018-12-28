#include <iostream>
#include <vector>
using namespace std;
typedef long long ll;

// Dinic's algorithm for max flow
// Source must be node 0, sink must be node n-1
// All edges are directed
// O(V^2 E) in general
// O(min(V^{2/3}, sqrt(E)) E) in graphs with unit capacities
// O(sqrt(V) E) in unit networks (e.g. bipartite matching)
struct Dinic {
	private:
	struct Edge {
		const int src;
		const int tar;
		const ll cap;
		ll flow;

		Edge(int a, int b, ll c) : src(a), tar(b), cap(c) {
			flow = 0;
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
	int n;


	// Update dist (building level graph)
	void calcDists() {
		for (int i = 0; i < n; ++i) dist[i] = n;
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

	// ######################################################################
	// #################### Use only the functions below ####################
	// ######################################################################
	public:

	void init(int nodes, int src, int sink) {
		n = nodes;
		flow_src = src;
		flow_tar = sink;
		conns.resize(n);
		dist.resize(n);
	}

	// Adds the given edge, and returns its index
	// Edges are directed. To add an undirected edge, call as
	// addEdge(src, tar, 2*cap, cap)
	int addEdge(int src, int tar, ll cap, ll flow = 0) {
		int i = edges.size();
		edges.emplace_back(src, tar, cap);
		edges.back().flow = flow;
		conns[src].push_back(i);
		conns[tar].push_back(i);
		return i;
	}

	// Push flow. Returns how much flow could be pushed
	ll pushFlow() {
		ll res = 0;
		while(true) {
			calcDists();
			if (dist[flow_src] == n) return res;
			res += dfsFlow(flow_src, 8*(ll)1e18);
		}
		return res;
	}

	ll getFlow(int edge_id) const {
		return edges[edge_id].flow;
	}
};

int main() {
	// Example usage, with undirected edges.
	int n, m;
	cin >> n >> m;
	int src, sink;
	cin >> src >> sink;
	--src; --sink;

	vector<ll> caps(m);
	Dinic dinic;
	dinic.init(n, src, sink);

	for (int i = 0; i < m; ++i) {
		int a, b;
		ll c;
		cin >> a >> b >> c;
		--a; --b;
		dinic.addEdge(a, b, 2*c, c);
		caps[i] = c;
	}
	
	ll flow = dinic.pushFlow();
	cout << "Flow: " << flow << '\n';
	cout << "Edges: ";
	for (int i = 0; i < m; ++i) {
		cout << dinic.getFlow(i) - caps[i] << ' '; // negative if flow from tar to src
	}
	cout << '\n';
}
