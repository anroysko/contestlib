#include <iostream>
#include <vector>
using namespace std;
using ll = long long;

// Dinic's algorithm for max flow
// Source must be node 0, sink must be node n-1
// All edges are directed
// O(V^2 E) in general
// O(min(V^{2/3}, sqrt{E}) E) in graphs with unit capacities
// O(sqrt{V} E) in unit networks (e.g. bipartite matching)
class Dinic {
	private:
		constexpr static ll INF = 8*(ll)1e18;
		struct Edge {
			const int s, t;
			const ll c; // capacity
			ll f; // flow

			Edge(int src, int tar, ll cap = INF, bool dir = 1)
				: s(src), t(tar), c(dir ? cap : 2*cap), f(dir ? 0 : cap) {}

			int getOth(int i) { return i == s ? t : s; }
			ll getCap(int i) { return i == s ? c - f : f; }
			void push(int i, ll am) { f += (i == s ? am : -am); }
		};

		const int source, sink, n;
		vector<Edge> edges;
		vector<vector<int>> conns;
		vector<int> dist;
		vector<bool> done;
		
		bool calcDists() {
			for (int i = 0; i < n; ++i) dist[i] = n;
			dist[sink] = 0;
			vector<int> que = {sink};
			for (int j = 0; j < que.size(); ++j) {
				int i = que[j];
				for (auto ei : conns[i]) {
					int t = edges[ei].getOth(i);
					if (edges[ei].getCap(t) == 0) continue;
					if (dist[t] == n) {
						dist[t] = dist[i] + 1;
						que.push_back(t);
					}
				}
			}
			return dist[source] < n;
		}
		ll dfsFlow(int i, ll cap = INF) {
			if (i == sink) return cap;
			ll res = 0;
			for (auto ei : conns[i]) {
				int t = edges[ei].getOth(i);
				if (done[t] || dist[t] != dist[i] - 1) continue;

				ll subcap = min(cap-res, edges[ei].getCap(i));
				if (subcap == 0) {
					if (cap == res) return res;
				} else {
					ll add = dfsFlow(t, subcap);
					res += add;
					edges[ei].push(i, add);
				}
			}
			if (res < cap) done[i] = true;
			return res;
		}
	public:
		Dinic(int nn, int s, int t)
			: n(nn), conns(nn), dist(nn), done(nn), source(s), sink(t) {}

		const Edge& getEdge(int j) const { return edges[j]; }
		void addEdge(int a, int b, ll c = INF, bool dir = 1) {
			edges.emplace_back(a, b, c, dir);
			conns[a].push_back(edges.size() - 1);
			conns[b].push_back(edges.size() - 1);
		}
		ll pushFlow() {
			ll res = 0;
			while(calcDists()) {
				for (int i = 0; i < n; ++i) done[i] = false;
				res += dfsFlow(source);
			}
			return res;
		}
};

// Example usage
int main() {
	int n, m;
	cin >> n >> m;

	Dinic dinic(n, 0, n-1);

	for (int i = 0; i < m; ++i) {
		int a, b;
		ll c;
		cin >> a >> b >> c;
		--a; --b;
		dinic.addEdge(a, b, c);
	}
	
	ll flow = dinic.pushFlow();
	cout << "Total flow: " << flow << '\n';
}
