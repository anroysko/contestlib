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
		ll getFlow(int ei) const { return edges[ei].f; }
};

class BipMatcher : private Dinic {
	private:
		const vector<bool> side;
	public:
		BipMatcher(const vector<bool>& sd) : Dinic(sd.size()+2, sd.size(), sd.size()+1), side(sd) {
			for (int i = 0; i < side.size(); ++i) {
				if (side[i]) addEdge(source, i, 1);
				else addEdge(i, sink, 1);
			}
		}

		void addEdge(int a, int b) {
			if (side[a]) swap(a, b);
			Dinic::addEdge(a, b, 1);
		}

		vector<pair<int, int>> getMaxMatching() {
			pushFlow();
			vector<pair<int, int>> res;
			for (int i = side.size(); i < edges.size(); ++i) {
				auto ed = solver.getEdge(i);
				if (ed.flow) res.emplace_back(ed.src, ed.tar);
			}
			return res;
		}

		vector<int> getMinVertexCover() {
			pushFlow();
			calcDists();
			vector<int> k;
			for (int i = 0; i < side.size(); ++i) {
				if (side[i] != (dist[i] < n)) k.push_back(i);
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
